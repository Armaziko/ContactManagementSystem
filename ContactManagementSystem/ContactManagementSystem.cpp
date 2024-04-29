#include <iostream>
#include <fstream>
#include "json.hpp"
#include <cstdlib>

struct Contacts
{
	std::string name;
	std::string phone;
	std::string email;
};

class CMS {
private: 
	using json = nlohmann::json;
public:
	void clearScreen() {
		std::cout << "\033[2J\033[1;1H";
	}
	void pauseScreen() {
		std::cout << std::endl;
		std::cout << "Press any button to continue...";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.get();
	}
	json readJSON() {
		std::ifstream file("storage.json");
		json data;
		file >> data;
		file.close();
		return data;
	}
	void writeJSON(const json& data) {
		std::ofstream file("storage.json");
		file << data;
		file.close();
	}
	void drawList() {
		json contacts = readJSON();
		for (const auto& contact : contacts["contacts"]) {
			std::cout << std::endl;
			std::cout << "Name: " << contact["name"] << std::endl;
			std::cout << "Phone number: " << contact["phone"] << std::endl;
			std::cout << "E-Mail address: " << contact["e-mail"] << std::endl;
		}
		pauseScreen();
		returnTo_mainMenu();
	}
	void addList() {
		Contacts conts;
		std::cout << "Add the name of the new contact: ";
		std::cin >> conts.name;
		std::cout << std::endl;
		bool repeater = true;
		while (repeater) {
			std::cout << "Add the phone number of the new contact: ";
			std::cin >> conts.phone;
			if (conts.phone.size() > 14 || conts.phone.size() < 7) repeater = true;
			else { repeater = false; break; }
			int i = 0;
			while (repeater) { if (conts.phone[i] < '0' && conts.phone[i] > '9') repeater = true;  break; }
			std::cout << std::endl;
		}
		repeater = true;
		int count = 0;
		while (repeater) {
			std::cout << "Add the email of the new contact: ";
			std::cin >> conts.email;
			if (conts.email.size() < 5) repeater = true;
			else { repeater = false; break; }
			int i = 0;
			while (repeater) { if (conts.email[i] == '@') count++;}
			if (count != 1) repeater = true;
			else repeater = false;
			std::cout << std::endl;
		}
		json newContact = {
			{"name", conts.name},
			{"phone", conts.phone},
			{"e-mail", conts.email}
		};
		json file = readJSON();
		file["contacts"].push_back(newContact);
		writeJSON(file);

		std::cout << "New contact has been added. Do you want to see the updated contacts list? (0 for No, 1 for Yes): ";
		int dial = -1;
		while (dial != 0 && dial != 1) std::cin >> dial;
		std::cout << std::endl;
		if (dial == 1) drawList();
		else returnTo_mainMenu();

	}
	void editList() {
		clearScreen();
		std::cout << "Your contacts: " << std::endl;
		json contacts = readJSON();
		int count = 0;
		for (const auto& contact : contacts["contacts"]) {
			count++;
			std::cout << std::endl;
			std::cout << "    Name: " << contact["name"] << std::endl;
			std::cout << "[" << count << "] " << "Phone number: " << contact["phone"] << std::endl;
			std::cout << "    E-Mail address: " << contact["e-mail"] << std::endl;
		}

		std::cout << "Which contact do you wish to remove: " << std::endl;
		int dial = count + 1;
		while (dial < 0 || dial > count) std::cin >> dial;
		contacts["contacts"].erase(contacts["contacts"].begin() + (dial-1));
		writeJSON(contacts);

		std::cout << "Contacts have been updated. Do you want to see the updated contacts list? (0 for No, 1 for Yes): ";
		dial = -1;
		while (dial != 0 && dial != 1) std::cin >> dial;
		std::cout << std::endl;

		if (dial == 1) drawList();
		else returnTo_mainMenu();

	}
	void returnTo_mainMenu() {
		std::cout << std::endl;
		int dial = -1;
		std::cout << "Do you want to get back to the main menu?(0 for No, 1 for Yes): ";
		dial = -1;
		while (dial != 0 && dial != 1) std::cin >> dial;

		if (dial == 1) mainMenu();
		exitApp();
	}
	void exitApp() {
		exit(EXIT_SUCCESS);
	}
	void mainMenu() {
		int dial = -1;
		while (dial != 1 && dial != 2 && dial != 3 && dial != 4) {
			std::cout << "Hey! Do you wish to see storage contacts or add contacts to your list or edit them?" << std::endl;
			std::cout << "[1] VIEW" << std::endl;
			std::cout << "[2] ADD" << std::endl;
			std::cout << "[3] EDIT" << std::endl;
			std::cout << "[4] EXIT" << std::endl;
			std::cin >> dial;
		}
		if (dial == 1) drawList();
		else if (dial == 2) addList();
		else if (dial == 3) editList();
		else exitApp();

		pauseScreen();
	}
};


int main()
{
	CMS* cms = new CMS;
	cms->mainMenu();
	delete cms;
}

