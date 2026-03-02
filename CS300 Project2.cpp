#include <iostream>
#include <fstream>
#include <sstream>
#include <list>

using namespace std;

struct Course {
    string courseNum;
    string courseTitle;
    list<string> coursePres;
};

struct Node {
    Course course;
    Node* left;
    Node* right;

    // Node constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // Initialize node with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }

};

class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
public:
    BinarySearchTree();
    void Insert(Course course);
    void Search(string courseNum);
    void printAllCompSci();
    void findCourse();
    void printAll();
};

// Initialize an empty BST with no root
BinarySearchTree::BinarySearchTree() {
    this->root = nullptr;
}


void BinarySearchTree::Insert(Course course) {
    // if the tree is empty add a new node to the root of this bst
    if (this->root == nullptr) {
        Node* newNode = new Node();
        newNode->course = course;
        this->root = newNode;
    }
    // find where to insert this node
    else {
        addNode(this->root, course);
    }
}


void BinarySearchTree::addNode(Node* node, Course course) {
    // if course is to the left of the node
    if (course.courseNum < node->course.courseNum) {
        // if there is no left node, add course to new left node
        if (node->left == nullptr) {
            Node* newNode = new Node();
            newNode->course = course;
            node->left = newNode;
        }
        // if there is left node, recursive call till you find empty leaf
        else {
            addNode(node->left, course);
        }
    }
    else {
        // if there is an open right leaf add to tree
        if (node->right == nullptr) {
            Node* newNode = new Node();
            newNode->course = course;
            node->right = newNode;
        }
        // recursively call until you find an open leaf
        else {
            addNode(node->right, course);
        }
    }
}

void BinarySearchTree::Search(string courseNum) {
    // Begin search with root of this tree
    Node* currNode = this->root;
    // Verify current node exists
    while (currNode != nullptr) {
        // If we find course, return the course details
        if (currNode->course.courseNum == courseNum) {
            cout << currNode->course.courseNum << ", " << currNode->course.courseTitle;
            cout << endl;
            cout << "Prerequisites: ";
            int i = 0;
            // using auto, and it we can use an iterator to iterate through the list to check if there needs to be a trailing comma
            for (auto it = currNode->course.coursePres.begin(); it != currNode->course.coursePres.end(); ++it) {
                // prints the courseNum from pres
                cout << *it;
                // checks if the next iterator is not the last item in pres, if its not add a trailin comma
                if (next(it) != currNode->course.coursePres.end()) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
        // if courseNum is in the left subtree
        if (courseNum < currNode->course.courseNum) {
            currNode = currNode->left;
        }
        // courseNum has to be in the right subtree
        else {
            currNode = currNode->right;
        }
    }
}

// Helper function to call begin inorder traversal
void BinarySearchTree::printAll() {
    inOrder(this->root);
}

void BinarySearchTree::inOrder(Node* node) {
    // verifies there is a node to check
    if (node != nullptr) {
        inOrder(node->left);

        // prints the main details for a course
        cout << node->course.courseNum << ", " << node->course.courseTitle << endl;

        inOrder(node->right);
    }
}

void BinarySearchTree::findCourse() {
    // Prompt user to enter course to find
    cout << "Enter a course number to search for." << endl;
    string courseNum;
    cin >> courseNum;
    // set all characters in courseNum to uppercase for case insensitive lookup
    for (char& c : courseNum) {
        c = toupper(c);
    }

    Search(courseNum);
}

void readFile(BinarySearchTree* bst) {

    string filename;
    // Prompts the user for the filename to be parsed for data
    cout << "Enter file name for file you wish to add. (Enter 'default' for the default class schedule): ";
    cin >> filename;

    // is the default file provided in supporting materials, will load properly when added to project folder
    if (filename == "default") {
        filename = "CS 300 ABCU_Advising_Program_Input.csv";
    }

    ifstream file(filename);

    // base case if no file exists
    if (!file) {
        cout << "No such file found" << endl;
    }

    string line;

    // for each line in the file
    while (getline(file, line)) {
        stringstream ss(line);
        // cell is the data in each col
        string cell;

        int col = 0;
        // new course and populate it
        Course course;

        while (getline(ss, cell, ',')) {
            // Sets course data for the first 2 columns in row
            if (col == 0) course.courseNum = cell;
            else if (col == 1) course.courseTitle = cell;
            // Adds all prerequite classes as long as format remains as csv of course nums
            else course.coursePres.push_back(cell);

            col++;
        }
        //insert the new course into the bst
        bst->Insert(course);

    }
}

void displayMenu() {
    // Displays the menu functionality
    cout << "1: Load Files." << endl;
    cout << "2: Print all Courses in the Computer Science Department." << endl;
    cout << "3: Print course details" << endl;
    cout << "9: Quit" << endl << endl;
}

int main()
{
    BinarySearchTree* bst;
    bst = new BinarySearchTree();

    clock_t ticks;



    int choice = 0;
    cout << "Welcome to the course planner" << endl << endl;
    while (choice != 9) {
        displayMenu();
        // Tells the user to input their selection
        cout << "What would you like to do? ";
        // Prints invalid input, if input is not a number.
        while ((!(cin >> choice)) || (choice != 1 && choice != 2 && choice != 3 && choice != 9)) {
            cout << choice << " is not a valid option." << endl << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            displayMenu();
        }

        // Switch on input to handle correct inputs
        switch (choice)
        {
        case 1:
            readFile(bst);
            break;

        case 2:
            cout << "Here is a sample Schedule: " << endl;
            bst->printAll();
            cout << endl;
            break;
        case 3:
            bst->findCourse();
            cout << endl;
            break;
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            exit(EXIT_SUCCESS);
            break;
        }
    }
}