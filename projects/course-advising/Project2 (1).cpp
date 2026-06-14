#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

// =========================
// Course Structure
// =========================
struct Course {
	string courseNumber;
	string courseTitle;
	vector<string> prerequisites;
};

// =========================
// Binary Search Tree Node
// =========================
struct Node {
	Course course;
	Node* left;
	Node* right;

	Node(Course c) {
		course = c;
		left = nullptr;
		right = nullptr;
	}
};

// =========================
// Binary Search Tree Class
// =========================
class BinarySearchTree {
private:
	Node* root;

	// insert helper (recursive)
	void insert(Node*& node, Course course) {
		if (node == nullptr) {
			node = new Node(course);
		}
		else if (course.courseNumber < node->course.courseNumber) {
			insert(node->left, course);
		}
		else {
			insert(node->right, course);
		}
	}

	// In-order traversal for sorted output
	void inOrder(Node* node) const {
		if (node == nullptr) return;

		inOrder(node->left);
		cout << node->course.courseNumber << ", "
			<< node->course.courseTitle << endl;
		inOrder(node->right);
	}

	// Search Helper
	Course* search(Node* node, string courseNumber) const {
		if (node == nullptr) return nullptr;

		if (node->course.courseNumber == courseNumber)
			return &(node->course);

		if (courseNumber < node->course.courseNumber)
			return search(node->left, courseNumber);
		else
			return search(node->right, courseNumber);
	}

public:
	BinarySearchTree() {
		root = nullptr;
	}

	void Insert(Course course) {
		insert(root, course);
	}

	void PrintAll() const {
		inOrder(root);
	}

	Course* Search(string courseNumber) const {
		return search(root, courseNumber);
	}
};

// =========================
// Convert string to uppercase
// =========================
string toUpper(string str) {
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

// =========================
// Load Courses From File
// =========================
void loadCourses(string filename, BinarySearchTree& bst) {

	ifstream file(filename);

	if (!file.is_open()) {
		cout << "Error opening file." << endl;
		return;
	}

	string line;

	while (getline(file, line)) {

		stringstream ss(line);
		string token;
		vector<string> tokens;

		while (getline(ss, token, ',')) {
			tokens.push_back(token);
		}

		if (tokens.size() < 2) {
			cout << "Invalid line format." << endl;
			continue;
		}

		Course course;
		course.courseNumber = toUpper(tokens[0]);
		course.courseTitle = tokens[1];

		for (size_t i = 2; i < tokens.size(); i++) {
			course.prerequisites.push_back(toUpper(tokens[i]));
		}

		bst.Insert(course);
	}

	file.close();
	cout << "Data loaded successfully." << endl;
}

// =========================
// Print Course Details
// =========================
void printCourse(BinarySearchTree& bst, string courseNumber) {

	courseNumber = toUpper(courseNumber);

	Course* course = bst.Search(courseNumber);

	if (course == nullptr) {
		cout << "Course not found." << endl;
		return;
	}

	cout << course->courseNumber << ", "
		<< course->courseTitle << endl;

	if (course->prerequisites.empty()) {
		cout << "Prerequisites: None" << endl;
	}
	else {
		cout << "Prerequisites: ";
		for (size_t i = 0; i < course->prerequisites.size(); i++) {
			cout << course->prerequisites[i];
			if (i < course->prerequisites.size() - 1)
				cout << ", ";
		}
		cout << endl;
	}
}

// =========================
// Main Program
// =========================
int main() {

	BinarySearchTree bst;
	int choice = 0;
	string filename;
	bool dataLoaded = false;

	cout << "Welcome to the course planner." << endl;

	while (choice != 9) {

		cout << "\n1. Load Data Structure." << endl;
		cout << "2. Print Course List." << endl;
		cout << "3. Print Course." << endl;
		cout << "9. Exit" << endl;
		cout << "\nWhat would you like to do? ";
		
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}

		switch (choice) {

		case 1:
			cout << "Enter file name: ";
			cin >> filename;
			loadCourses(filename, bst);
			dataLoaded = true;
			break;

		case 2:
			if (!dataLoaded) {
				cout << "Please load data first." << endl;
			}
			else {
				cout << "\nHere is a sample schedule:\n" << endl;
				bst.PrintAll();
			}
			break;

		case 3:
			if (!dataLoaded) {
				cout << "Please load data first." << endl;
			}
			else {
				string courseNumber;
				cout << "What course do you want to know about? ";
				cin >> courseNumber;
				printCourse(bst, courseNumber);
			}
			break;

		case 9:
			cout << "Thank you for using the course planner!" << endl;
			break;

		default:
			cout << choice << " is not a valid option." << endl;
		}
	}

	return 0;
}