//This program is made to gather information on the 
//orders in a business. This can also output a recipt

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;
int orderNum = 0;
ofstream itemsListEdit;
ifstream itemsListGet;
ofstream orderListEdit;
ifstream orderListGet;
vector<string> itemList;

void ShowItemList() {
	itemList.clear();
	itemsListGet.open("database.csv");
	string line;
	while (getline(itemsListGet, line)) {
		itemList.push_back(line);
	}
	for (int i = 0; i < itemList.size(); ++i) {
		size_t commaPos = itemList[i].find(",");
		string itemName = itemList[i].substr(0, commaPos);
		double itemPrice = stod(itemList[i].substr(commaPos + 1));

		cout << i + 1 << ". " << itemName << " - $" << std::fixed << std::setprecision(2) << itemPrice << "\n";
	}
	itemsListGet.close();
}

void RemoveOrderItems(int orderNum) {
	string orderDB = "orderDatabase.csv";
	ifstream orderListGet(orderDB);
	ofstream tempFile("temp.csv", ios::trunc); // Create a temporary file

	string line;
	bool orderFound = false;
	while (getline(orderListGet, line)) {
		size_t commaPos = line.find(",");
		int orderNumber = stoi(line.substr(0, commaPos));

		if (orderNumber == orderNum) {
			orderFound = true;
			size_t lastCommaPos = line.rfind(",");
			double itemPrice = stod(line.substr(lastCommaPos + 1));
			string itemName = line.substr(commaPos + 1, lastCommaPos - commaPos - 1);

			cout << "Removing " << itemName << " from order #" << orderNum << " - $" << std::fixed << std::setprecision(2) << itemPrice << "\n";
		}
		else {
			tempFile << line << "\n"; // Write lines for other orders to temporary file
		}
	}

	orderListGet.close();
	tempFile.close();

	if (orderFound) {
		remove(orderDB.c_str());       // Delete original file
		rename("temp.csv", orderDB.c_str()); // Rename temporary file to original file name
	}
	else {
		remove("temp.csv"); // If order is not found, just delete the temporary file
	}
}

void AddOrderItems(int orderNum) {
	orderListEdit.open("orderDatabase.csv", std::ios::app);
	string itemName;
	double itemPrice;

	int selectedItemIndex;
	string addAnotherItem = "Y";
	while (addAnotherItem == "Y" || addAnotherItem == "y") {

		ShowItemList();
		cout << "\nEnter the number of the item you want to add to the order: ";
		cin >> selectedItemIndex;
		selectedItemIndex--;
		if (selectedItemIndex >= 0 && selectedItemIndex < itemList.size()) {
			size_t commaPos = itemList[selectedItemIndex].find(",");
			string itemName = itemList[selectedItemIndex].substr(0, commaPos);
			double itemPrice = stod(itemList[selectedItemIndex].substr(commaPos + 1));

			cout << "Added " << itemName << " to order #" << orderNum << " - $" << std::fixed << std::setprecision(2) << itemPrice << "\n";

			orderListEdit << orderNum << "," << itemName << "," << itemPrice << "\n";
		}
		else {
			cout << "Invalid selection.\n";
			cout << "Add another item? (y/n): ";
			cin >> addAnotherItem;
		}
		cout << "Add another item? (y/n): ";
		cin >> addAnotherItem;
	}
	orderListEdit.close();
}

void CreateOrder() {

	orderListGet.open("orderDatabase.csv");            //open orders database
	int highestOrderNum = 0;
	string line;

	while (getline(orderListGet, line)) {
		size_t commaPos = line.find(",");
		int orderNumber = stoi(line.substr(0, commaPos));    
		if (orderNumber > highestOrderNum) {
			highestOrderNum = orderNumber;
		}
	}

	orderNum = highestOrderNum + 1;
	AddOrderItems(orderNum);

	orderListGet.close();    //close orders database
}

void EditOrder() {
	int choice;
	int orderNum;
	cout << "Enter Order Number: ";
	cin >> orderNum;
	cout << "\nPlease input the number of your choice : \n1. Add items to order\n2. Remove Item from order\n";
	cin >> choice;
	if (choice == 1) {
		AddOrderItems(orderNum);
	}
	if (choice == 2) {
		RemoveOrderItems(orderNum);
	}
	else if (choice != 1 && choice != 2){
		cout << "Choice Invalid. Exiting edit order.\n";
	}

}

void GetReceipt() {
	int orderNum;
	cout << "Enter Order Number: ";
	cin >> orderNum;

	ifstream orderListGet("orderDatabase.csv");
	string line;
	double totalPrice = 0;

	while (getline(orderListGet, line)) {
		size_t commaPos1 = line.find(",");
		size_t commaPos2 = line.find(",", commaPos1 + 1);

		int orderNumber = stoi(line.substr(0, commaPos1));
		string itemName = line.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);
		double itemPrice = stod(line.substr(commaPos2 + 1));

		if (orderNumber == orderNum) {
			cout << itemName << " - $" << std::fixed << std::setprecision(2) << itemPrice << "\n";
			totalPrice += itemPrice;
		}
	}

	cout << "Total Price: $" << std::fixed << std::setprecision(2) << totalPrice << "\n";
	orderListGet.close();
}


void AddItem() {
	ShowItemList();
	itemsListEdit.open("database.csv", std::ios::app);
	string itemName;
	double itemPrice;

	cout << "New Item Name: ";
	cin.ignore(); 
	getline(cin, itemName);
	itemsListEdit << itemName << ",";

	cout << "New Item Price: ";
	cin >> itemPrice;
	itemsListEdit << itemPrice << "\n";

	itemsListEdit.close();
}

void RemoveItem() {
	ShowItemList();
	itemsListEdit.open("database.csv");
	string itemName;

	cout << "Enter the name of the item you want to remove: ";
	cin.ignore();
	getline(cin, itemName);

	vector<string> updatedItemList;
	string line;
	bool itemRemoved = false;
	while (getline(itemsListGet, line)) {
		size_t commaPos = line.find(",");
		string currentItemName = line.substr(0, commaPos);

		if (currentItemName != itemName) {
			updatedItemList.push_back(line);
		}
		else {
			itemRemoved = true;
		}
	}

	itemsListEdit.close();
	itemsListEdit.open("database.csv", ios::trunc); // Open file in truncate mode to clear its contents

	for (const string& updatedLine : updatedItemList) {
		itemsListEdit << updatedLine << "\n";
	}

	itemsListEdit.close();

	if (itemRemoved) {
		cout << "Item '" << itemName << "' removed from database.\n";
	}
	else {
		cout << "Item '" << itemName << "' not found in the database.\n";
	}
}

	

int main() {

	int choice;
	cout << "Please input the number of your choice: \n1. New Order\n2. Add Item\n3. Remove Item\n4. Show Item List\n5. Edit Order\n6. Get Receipt\n0. Exit\n";
	cin >> choice;

	while (choice != 0) {

		if (choice == 1) {
			CreateOrder();
		}
		if (choice == 2) {
			AddItem();
		}
		if (choice == 3) {
			RemoveItem();
		}
		if (choice == 4) {
			ShowItemList();
		}
		if (choice == 5) {
			EditOrder();
		}
		if (choice == 6) {
			GetReceipt();
		}

		cout << "Please input the number of your choice: \n1. New Order\n2. Add Item\n3. Remove Item\n4. Show Item List\n5. Edit Order\n6. Get Receipt\n0. Exit\n";
		cin >> choice;
	}
	return 0;
}