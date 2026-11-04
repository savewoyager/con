#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<windows.h>
#include<functional>
#include<pugixml.hpp>
#include<filesystem>
#include<iomanip>
#include<ctime>
#include<vector>
#include <algorithm>
using namespace std;
using namespace pugi;

xml_document vfs_doc;

const char* username = getenv("USERNAME");
static int errors = 0;
string current_path = "/";
xml_node find_directory(xml_node root, const string& path);

//=====================================
string json_escape(const string& value)
{
	string result;

	for (unsigned char c : value)
	{
		switch (c)
		{
		case '"':  result += "\\\""; break;
		case '\\': result += "\\\\"; break;
		case '\n': result += "\\n"; break;
		case '\r': result += "\\r"; break;
		case '\t': result += "\\t"; break;
		case '\b': result += "\\b"; break;
		case '\f': result += "\\f"; break;

		default:
			if (c < 0x20)
			{
				stringstream ss;
				ss << "\\u"
					<< hex << setw(4) << setfill('0')
					<< static_cast<int>(c);

				result += ss.str();
			}
			else
				result += static_cast<char>(c);
		}
	}

	return result;
}
void ls(string instr, const string& path)
{
	stringstream ss(instr);
	string command, arg;

	ss >> command;

	bool f_l = false;
	bool f_a = false;
	bool f_h = false;

	// Обработка флагов
	while (ss >> arg)
	{
		if (arg.empty() || arg[0] != '-')
		{
			cerr << "ls: invalid argument: "
				<< arg << endl;
			errors++;
			return;
		}

		for (size_t i = 1; i < arg.size(); i++)
		{
			switch (arg[i])
			{
			case 'l':
				f_l = true;
				break;

			case 'a':
				f_a = true;
				break;

			case 'h':
				f_h = true;
				break;

			default:
				cerr << "ls: неизвестный флаг: "
					<< arg << '\n';
				errors++;
				return;
			}
		}
	}

	// Получаем текущую директорию из XML
	xml_node root = vfs_doc.document_element();

	xml_node directory =
		find_directory(root, current_path);

	if (!directory)
	{
		cerr << "ls: текущий каталог не найден\n";
		errors++;
		return;
	}

	// Создаём JSON-запрос
	ofstream request(
		"runtime/ls_request.json",
		ios::out | ios::trunc
	);

	if (!request.is_open())
	{
		cerr << "Не удалось создать JSON-запрос\n";
		errors++;
		return;
	}

	request << "{\n";

	request << "\"command\": \"ls\",\n";

	request << "\"flags\": {\n";
	request << "\"l\": " << (f_l ? "true" : "false") << ",\n";
	request << "\"a\": " << (f_a ? "true" : "false") << ",\n";
	request << "\"h\": " << (f_h ? "true" : "false") << "\n";
	request << "},\n";

	request << "\"entries\": [\n";

	bool first = true;

	// Собираем информацию о файлах и каталогах
	for (xml_node node : directory.children())
	{
		if (node.type() != node_element)
			continue;

		string type = node.name();

		if (type != "file" && type != "directory")
			continue;

		string name = node.attribute("name").value();

		size_t size = 0;

		if (type == "file")
			size = string(node.child_value()).size();

		if (!first)
			request << ",\n";

		first = false;

		request << "{";
		request << "\"name\": \""
			<< json_escape(name) << "\",";
		request << "\"type\": \""
			<< type << "\",";
		request << "\"size\": "
			<< size;
		request << "}";
	}

	request << "\n]\n";
	request << "}\n";

	request.close();

	if (!request)
	{
		cerr << "Ошибка записи JSON\n";
		errors++;
		return;
	}
	// Запускаем Python
	int result = system(
		"py python/ls.py runtime/ls_request.json"
	);


	if (result != 0)
	{
		cerr << "Ошибка выполнения Python-модуля\n";
		errors++;
	}
}

void who(string instr)
{
	stringstream ss(instr);
	string command, arg;
	ss >> command;
	if (ss >> arg)
	{
		cerr << "who: неизвестный аргумент: " << arg << endl;
		errors++;
		return;
	}
	int result = system("runtime\\who.exe");
	if (result != 0)
	{
		cerr << "Ошибка выполнения Go-модуля\n";
		errors++;
	}
}

void cd(string instr, const string& path)
{
	stringstream ss(instr);
	string command, arg;

	ss >> command;

	if (!(ss >> arg))
		arg = "/";

	// Создаём JSON-запрос
	ofstream request("runtime/cd_request.json");

	if (!request.is_open())
	{
		cerr << "cd: не удалось создать JSON-запрос\n";
		errors++;
		return;
	}

	request << "{\n";
	request << "\"current_path\": \""
		<< json_escape(current_path) << "\",\n";
	request << "\"input_path\": \""
		<< json_escape(arg) << "\"\n";
	request << "}\n";

	request.close();

	if (!request)
	{
		cerr << "cd: ошибка записи JSON\n";
		errors++;
		return;
	}

	// Запускаем JavaScript
	int result = system(
		"node javascript/cd.js "
		"runtime/cd_request.json "
		"runtime/cd_result.txt"
	);

	if (result != 0)
	{
		cerr << "cd: ошибка выполнения JavaScript-модуля\n";
		errors++;
		return;
	}

	// Получаем нормализованный путь
	ifstream response("runtime/cd_result.txt");

	if (!response.is_open())
	{
		cerr << "cd: не удалось получить результат JavaScript\n";
		errors++;
		return;
	}

	string new_path;
	getline(response, new_path);
	response.close();

	if (new_path.empty())
	{
		cerr << "cd: получен пустой путь\n";
		errors++;
		return;
	}

	// Проверяем существование каталога в XML
	xml_node root = vfs_doc.document_element();

	xml_node target = find_directory(root, new_path);

	if (!target)
	{
		cerr << "cd: каталог не найден: " << arg << '\n';
		errors++;
		return;
	}

	// Обновляем текущий путь
	current_path = new_path;
}




//=====================================
string normalize_path(const string& path)
{
	vector<string> parts;
	string full_path;

	if (!path.empty() && path[0] == '/')
		full_path = path;
	else if (current_path == "/")
		full_path = "/" + path;
	else
		full_path = current_path + "/" + path;

	stringstream ss(full_path);
	string part;

	while (getline(ss, part, '/'))
	{
		if (part.empty() || part == ".")
			continue;

		if (part == "..")
		{
			if (!parts.empty())
				parts.pop_back();
		}
		else
			parts.push_back(part);
	}

	string result = "/";
	for (size_t i = 0; i < parts.size(); i++)
	{
		if (i > 0)
			result += "/";
		result += parts[i];
	}

	return result;
}

xml_node find_entry(xml_node parent, const string& name)
{
	for (xml_node node : parent.children())
	{
		if (node.type() != node_element)
			continue;

		string type = node.name();
		if ((type == "file" || type == "directory") &&
			string(node.attribute("name").value()) == name)
			return node;
	}

	return xml_node();
}

xml_node find_entry_by_path(const string& path)
{
	string normalized = normalize_path(path);

	if (normalized == "/")
		return vfs_doc.document_element();

	size_t pos = normalized.find_last_of('/');
	string parent_path = normalized.substr(0, pos);
	string name = normalized.substr(pos + 1);

	if (parent_path.empty())
		parent_path = "/";

	xml_node parent = find_directory(
		vfs_doc.document_element(), parent_path);

	if (!parent)
		return xml_node();

	return find_entry(parent, name);
}

xml_node find_parent_by_path(const string& path)
{
	string normalized = normalize_path(path);

	if (normalized == "/")
		return xml_node();

	size_t pos = normalized.find_last_of('/');
	string parent_path = normalized.substr(0, pos);

	if (parent_path.empty())
		parent_path = "/";

	return find_directory(vfs_doc.document_element(), parent_path);
}


bool load_vfs(const string& path)
{
	cout << "[VFS] Загрузка из: " << path << '\n';

	xml_parse_result result = vfs_doc.load_file(path.c_str());
	if (!result)
	{
		cerr << "[VFS ERROR] Не получилось открыть "
			<< "или разобрать XML-файл: "
			<< path << '\n';
		cerr << "[VFS ERROR] Причина: "
			<< result.description() << '\n';
		return false;
	}

	cout << "[VFS] Файл открылся.\n";

	// Проверяем корневой элемент.
	xml_node root = vfs_doc.document_element();

	if (!root || string(root.name()) != "vfs")
	{
		cerr << "[VFS ERROR] Неправильный корневой элемент.\n";
		return false;
	}
	return true;
}
xml_node find_directory(xml_node root, const string& path)
{ // поиск нужной директории
	xml_node current = root;
	if (path == "/")
		return current;
	stringstream ss(path);
	string part;
	while (getline(ss, part, '/'))
	{
		if (part.empty())
			continue;
		xml_node next;
		for (xml_node node : current.children("directory"))
		{
			if (string(node.attribute("name").value()) == part)
			{
				next = node;
				break;
			}
		}
		if (!next)
			return xml_node();
		current = next;
	}
	return current;
}
void command_prompt()
{
	printf("%s@%s:~$ ", username, username);
}

void cp(string instr)
{
	stringstream ss(instr);
	string command, arg;
	vector<string> args;

	ss >> command;

	bool recursive = false;

	while (ss >> arg)
	{
		if (arg == "-r" || arg == "-R")
			recursive = true;
		else if (!arg.empty() && arg[0] == '-')
		{
			cerr << "cp: неизвестный флаг: " << arg << '\n';
			errors++;
			return;
		}
		else
			args.push_back(arg);
	}

	if (args.size() != 2)
	{
		cerr << "Использование: cp [-r] источник назначение\n";
		errors++;
		return;
	}

	string src_path = normalize_path(args[0]);
	string dst_path = normalize_path(args[1]);

	xml_node src = find_entry_by_path(src_path);

	if (!src || src == vfs_doc.document_element())
	{
		cerr << "cp: источник не найден: " << args[0] << '\n';
		errors++;
		return;
	}

	string type = src.name();

	if (type == "directory" && !recursive)
	{
		cerr << "cp: пропущена директория (не указан -r): "
			<< args[0] << '\n';
		errors++;
		return;
	}

	if (type != "file" && type != "directory")
	{
		cerr << "cp: неподдерживаемый тип объекта\n";
		errors++;
		return;
	}

	xml_node dst = find_entry_by_path(dst_path);
	xml_node target_parent;
	string target_name;

	if (dst)
	{
		if (string(dst.name()) != "directory")
		{
			cerr << "cp: назначение не является директорией: "
				<< args[1] << '\n';
			errors++;
			return;
		}

		target_parent = dst;
		target_name = src.attribute("name").value();
	}
	else
	{
		target_parent = find_parent_by_path(dst_path);

		size_t pos = dst_path.find_last_of('/');
		target_name = dst_path.substr(pos + 1);
	}

	if (!target_parent || target_name.empty())
	{
		cerr << "cp: каталог назначения не найден: "
			<< args[1] << '\n';
		errors++;
		return;
	}

	// Нельзя копировать объект в самого себя или внутрь него.
	string source_prefix = src_path + "/";
	if (dst_path == src_path ||
		(dst_path.size() > source_prefix.size() - 1 &&
			dst_path.compare(0, source_prefix.size(), source_prefix) == 0))
	{
		cerr << "cp: нельзя копировать объект внутрь самого себя\n";
		errors++;
		return;
	}

	if (find_entry(target_parent, target_name))
	{
		cerr << "cp: объект уже существует: "
			<< target_name << '\n';
		errors++;
		return;
	}

	// Создаём XML-запрос для Java
	xml_document request_doc;

	xml_node request_root =
		request_doc.append_child("request");

	request_root.append_attribute("target") =
		target_name.c_str();

	xml_node source_wrapper =
		request_root.append_child("source");

	source_wrapper.append_copy(src);

	// Сохраняем запрос
	if (!request_doc.save_file("runtime/cp_request.xml"))
	{
		cerr << "cp: ошибка создания XML-запроса\n";
		errors++;
		return;
	}

	// Запускаем Java
	int result = system(
		"java -cp runtime CpModule "
		"runtime\\cp_request.xml runtime\\cp_result.xml"
	);

	if (result != 0)
	{
		cerr << "cp: ошибка выполнения Java-модуля\n";
		errors++;
		return;
	}

	// Загружаем результат
	xml_document result_doc;

	if (!result_doc.load_file("runtime/cp_result.xml"))
	{
		cerr << "cp: ошибка чтения результата Java\n";
		errors++;
		return;
	}

	// Получаем скопированный XML-узел
	xml_node result_root = result_doc.document_element();
	xml_node copied_node;

	for (xml_node node : result_root.children())
	{
		if (node.type() == node_element)
		{
			copied_node = node;
			break;
		}
	}

	if (!copied_node)
	{
		cerr << "cp: Java не вернула скопированный объект\n";
		errors++;
		return;
	}

	// Добавляем копию в виртуальную файловую систему
	target_parent.append_copy(copied_node);

	cout << "Скопировано: " << args[0]
		<< " -> " << args[1] << '\n';
}

void kill(string instr)
{
	stringstream ss(instr);
	string command;
	ss >> command;
	string arguments;
	getline(ss, arguments);
	ofstream request("runtime/kill_request.txt");
	request << arguments;
	request.close();
	system("dotnet runtime/KillModule.dll runtime/kill_request.txt runtime/kill_result.txt");
	ifstream result("runtime/kill_result.txt");
	string status;
	result >> status;
	if (status == "ERROR")
		errors++;
}
void rev(string instr, const string& path) 
{
	string filename;
	stringstream ss(instr);
	ss >> filename;
	if (!(ss >> filename)) 
	{
		cerr << "Укажите файл\n";
		return;
	}
	xml_document& doc = vfs_doc;
	xml_node root = doc.document_element();		
	xml_node directory = find_directory(root, current_path);
	xml_node find_file;
	for (xml_node node : directory.children())
	{
		if (node.type() != node_element)
			continue;

		string name = node.attribute("name").value();
		string type = node.name();

		if (type != "directory" && type != "file")
			continue;
		if (name[0] == '.')
			continue;
		if (name == filename)
			find_file = node;
	}
	if (!find_file) 
	{
		cerr << "Файл не найден\n";
		return;
	}
	string content = find_file.child_value();

	// Записываем содержимое файла во временный файл
	ofstream input(
		"runtime/rev_input.txt",
		ios::out | ios::binary | ios::trunc
	);

	if (!input.is_open())
	{
		cerr << "Не удалось создать временный файл\n";
		errors++;
		return;
	}

	input.write(content.data(), content.size());
	input.close();

	if (!input)
	{
		cerr << "Ошибка записи временного файла\n";
		errors++;
		return;
	}

	// Запускаем Rust-модуль
	int result = system(
		"runtime\\rev.exe runtime\\rev_input.txt"
	);

	if (result != 0)
	{
		cerr << "Ошибка выполнения Rust-модуля\n";
		errors++;
	}
}
void not_found(string instr)
{
	ofstream request("runtime/notfound_request.txt");
	request << instr;
	request.close();

	system("java -jar runtime/NotFound.jar runtime/notfound_request.txt");
}
bool execute(string command, const string& path)
{
	string inst;
	stringstream ss(command);
	if (!(ss >> inst))
		return true;
	if (inst == "ls")
		ls(command, path);
	else if (inst == "cd")
		cd(command, path);
	else if (inst == "who")
		who(command);
	else if (inst == "rev")
		rev(command, path);
	else if (inst == "cp")
		cp(command);
	else if (inst == "exit")
	{
		kill(command);
		return false;
	}
	else 
	{
		not_found(command);
		errors++;
		return true;
	}

	return true;
}
int main(int argc, char* argv[])
{
	cout << "To run command as administrator (user " << '"' << "root" << '"' \
		<< "), use " << '"' << "sudo <command>" << '"' << ".\nSee " \
		<< '"' << "man sudo_root" << '"' << " for details.\n";
	setlocale(LC_ALL, "Russian");

	if (argc != 3) 
	{
		cerr << "Usage: emulator.exe <VFS_path> <startup_script>\n";
		return 1;
	}

	string vfs_path = argv[1];
	string script_path = argv[2];
	cout << "VFS path: " << vfs_path << endl;
	cout << "Startup script: " << script_path << '\n';
    // выполнение проверки загрузки XML
    if (!load_vfs(vfs_path))
        return 1;

	ifstream file(script_path);
	if (!file.is_open()) 
	{
		cerr << "Error: cannot open script: " << script_path << endl;
		return 1;
	}

	string line = "";

	while (getline(file, line))
	{	// Пропускаем пустые строки и комменты
		size_t pos = line.find_first_not_of(" \t\r\n");
		if (pos == string::npos || line[pos] == '#')
			continue;
		// Показываем команду как будто ее ввел пользователь
		command_prompt();
		cout << line << '\n';

		if (!execute(line, vfs_path))
		{
			file.close();
			if (errors > 0)
				cout << "В ходе работы программы были обнаружены ошибки\n";
		}
			
	}
	file.close();
	if (errors > 0) 
		cout << "В ходе работы программы были обнаружены ошибки\n";
	while (true) 
	{
		command_prompt();
		if (!getline(cin, line))
			break; 
		if (!execute(line, vfs_path))
			break;
	}
}