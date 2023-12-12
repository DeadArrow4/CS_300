#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>

using namespace std;

// Define a Course structure to hold course information
struct Course {
    std::string courseId;
    std::string courseName;
    std::vector<std::string> prereq;
    Course() {
    }
    Course(std::string id, std::string name, std::vector<std::string> prerequisites) : courseId(std::move(id)), courseName(std::move(name)),prereq(std::move(prerequisites)){}
};

// Internal structure for tree node
struct Node {
    Course data;
    Node* left;
    Node* right;

    // default constructor
    explicit Node(Course course) : data(std::move(course)), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {

private:
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);
    Node* removeNode(Node* node, string courseId);
    Node* minValueNode(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void PostOrder();
    void PreOrder();
    void Insert(Course course);
    void Remove(string courseId);
    Course Search(string courseId);
    void printCourseList();
    void printCourseInformation(const std::string& courseNumber);
};


BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}


BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}


void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root
    inOrder(root);
}

void BinarySearchTree::PostOrder() {
    // postOrder root
    postOrder(root);

}



void BinarySearchTree::PreOrder() {
    // preOrder root
    preOrder(root);
}

void BinarySearchTree::Insert(Course course) {
    // if root equal to nullptr
        if (root == nullptr) {
            // root is equal to new node course
            root = new Node(course);
        }
    // else
        else {
            // add Node root and course
            this->addNode(root, course);
        }
}


void BinarySearchTree::Remove(string courseId) {
    // remove node root courseId
    root = removeNode(root, courseId);
}


Course BinarySearchTree::Search(string courseId) {

    // convert courseID to upper case for use in comparison
    transform(courseId.begin(), courseId.end(), courseId.begin(), ::toupper);


    // set current node equal to root
    Node* current = root;

    // keep looping downwards until bottom reached or matching bidId found
    while (current != nullptr) {
        // if match found, return current bid
        if (current->data.courseId.compare(courseId) == 0) {
            return current->data;
        }
        // if bid is smaller than current node then traverse left
        else if (courseId.compare(current->data.courseId) < 0) {
            current = current->left;
        }
        // else larger so traverse right
        else {
            current = current->right;
        }
    }
    Course data;
    return data;
}

void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (node->data.courseId.compare(course.courseId) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            addNode(node->left, course);
        }
    }
    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        //else
        else {
            // recurse down the left node
            addNode(node->right, course);
        }
    }
}
void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //InOrder not left
        inOrder(node->left);
        //output
        std::cout << node->data.courseId << ", " << node->data.courseName << std::endl;
        //InOder right
        inOrder(node->right);
    }
}
void BinarySearchTree::postOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //postOrder left
        postOrder(node->left);
        //postOrder right
        postOrder(node->right);
        //output
        std::cout << node->data.courseId << ", " << node->data.courseName << std::endl;
    }

}

void BinarySearchTree::preOrder(Node* node) {

  //if node is not equal to null ptr
    if (node != nullptr) {
        //output
        std::cout << node->data.courseId << ", " << node->data.courseName << std::endl;
        //postOrder left
        postOrder(node->left);
        //postOrder right
        postOrder(node->right);
    }
}

Node* BinarySearchTree::removeNode(Node* node, string courseId) {
    if (node == nullptr) {
        return node; // Base case: empty tree or bid not found
    }

    if (courseId.compare(node->data.courseId) < 0) {
        node->left = removeNode(node->left, courseId); // Go to the left subtree
    }
    else if (courseId.compare(node->data.courseId) > 0) {
        node->right = removeNode(node->right, courseId); // Go to the right subtree
    }
    else {
        // Bid found, proceed to remove it
        if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        else if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }

        // Node with two children: Get the inorder successor (smallest in the right subtree)
        Node* temp = minValueNode(node->right);

        // Copy the inorder successor's data to this node
        node->data = temp->data;

        // Delete the inorder successor
        node->right = removeNode(node->right, temp->data.courseId);
    }
    return node;
}


Node* BinarySearchTree::minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

// Function to load data from the file into the binary tree
void loadDataStructure(BinarySearchTree& courseTree, const std::string& filename) {
    std::cout << "Loading input file " << filename << std::endl;

    std::ifstream file(filename);
    std::string currentLine;
    try {
        while (std::getline(file, currentLine)) {
            std::stringstream ss(currentLine);
            std::string word, id, name;
            std::vector<std::string> prerequisites;
            int index = 0;
            while (!ss.eof()) {
                getline(ss, word, ',');
                word = std::regex_replace(word, std::regex(R"(\r\n|\r|\n)"), "");

                if (index == 0) {
                    id = word;
                }
                else if (index == 1) {
                    name = word;
                }
                else {
                    prerequisites.push_back(word);
                }
                index++;
            }
                Course tempCourse(id, name, prerequisites);
                courseTree.Insert(tempCourse);
        }
    }
    catch (std::ifstream::failure& e) {
        std::cerr << e.what() << std::endl;
    }

    file.close();
}

// Function to print courses in alphabetical order
void BinarySearchTree::printCourseList() {
    std::cout << "Courses in alphabetical order:" << std::endl;
    inOrder(root);
}
void displayBid(Course data) {
    std::vector<std::string> coursePrerequisites = data.prereq;
    std::string prerequisites;
    // formats output for prerequisites, if there are any
    if (coursePrerequisites.size() == 1) {
        prerequisites = data.prereq[0];
    }
    else if (coursePrerequisites.size() > 1) {
        for (int i = 0; i < coursePrerequisites.size() - 1; i++) {
            prerequisites += coursePrerequisites[i] + ", ";
        }
        prerequisites += coursePrerequisites.back();
    }
    else {
        prerequisites = "n/a";
    }

    // prints out the course details
    std::cout << data.courseId << ", " << data.courseName << std::endl;
    std::cout << "Prerequisites: " << prerequisites << std::endl;
}

int main() {
    std::string filename;
    string courseNumber;

    BinarySearchTree courseTree;

    Course data;
    //courseTree = new BinarySearchTree();

    int choice=0;
    while (choice != 4) {
        cout << "Menu:" << endl;
        cout << "  1. Load File" << endl;
        cout << "  2. Print List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  4. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter the file name containing course data: ";
                cin >> filename;
                loadDataStructure(courseTree, filename);
                cout << "Data loaded successfully." << endl;
                break;

            case 2:

                courseTree.printCourseList();
                break;

            case 3:
                std::cout << "Enter the course number: ";
                std::cin >> courseNumber;
                data = courseTree.Search(courseNumber);
                if (!data.courseId.empty()) {
                    displayBid(data);
                }
                else {
                    cout << "Course Id " << courseNumber << " not found." << endl;
                }
                break;

        case 4:

                cout << "Exiting program." << endl;
                break;

        default:

                cout << "Invalid choice. Please enter a valid option." << endl;
                break;
        }
    } while (choice != 4);

    return 0;
}