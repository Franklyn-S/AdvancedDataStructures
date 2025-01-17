// Implementing Red-Black Tree in C++

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

struct Node {
  int data;
  Node *parent;
  Node *left;
  Node *right;
  int color;
};

typedef Node *NodePtr;

bool isNodePtrValid(NodePtr node) {
  return node != nullptr && node->data > 0 && node != NULL && node->data < 38484704;
}

bool isNodeValid(Node node) {
  return node.data > 0 && node.data < 38484704;
}

NodePtr deepCopyNode(NodePtr node) {
  NodePtr newNode = new Node();
  if (isNodePtrValid(node)) {
    newNode->data = node->data;
    newNode->color = node->color;
    newNode->parent = node->parent;
    newNode->left = new Node();
    if (isNodePtrValid(node->left)) {
      newNode->left = deepCopyNode(node->left);
    }
    newNode->right = new Node();
    if (isNodePtrValid(node->right)) {
      newNode->right = deepCopyNode(node->right);
    }
  }
  return newNode;
}

class RedBlackTree { 
  private:
    NodePtr root;
    NodePtr TNULL;

    RedBlackTree* getTreeByVersion(int version) {
      if (version >= this->version) {
        return this;
      }
      
      RedBlackTree *tree;
      int versionToGoBack = this->version - version;
      tree = this->previousTree;
      versionToGoBack--;

      while (versionToGoBack > 0 && tree != nullptr && isNodePtrValid(tree->getRoot())) {
        tree = tree->previousTree;
        versionToGoBack--;
      }
      return tree;

    }

    // Sucessor
    NodePtr sucessorHelper(NodePtr node, int key) {
      NodePtr returnNode = TNULL;
      returnNode->data = numeric_limits<int>::max();
      if (node != TNULL) {
        returnNode = sucessorHelper(node->left, key);
        if (node->data > key) {
          return node;
        }
        returnNode = sucessorHelper(node->right, key);
      }
      return returnNode;
    }

    // Preorder
    void preOrderHelper(NodePtr node) {
      if (node != TNULL) {
        cout << node->data << " ";
        preOrderHelper(node->left);
        preOrderHelper(node->right);
      }
    }

    // Inorder
    void inOrderHelper(NodePtr node, int depth ) {
      if (isNodePtrValid(node)) {
        int nextDepth = depth + 1;
        inOrderHelper(node->left, nextDepth);
        cout << node->data << ",";
        cout << depth << ",";
        string sColor = node->color ? "R" : "N";
        cout << sColor << " ";
        inOrderHelper(node->right, nextDepth);
      }
    }

    // Post order
    void postOrderHelper(NodePtr node) {
      if (node != TNULL) {
        postOrderHelper(node->left);
        postOrderHelper(node->right);
        cout << node->data << " ";
      }
    }

    NodePtr searchTreeHelper(NodePtr node, int key) {
      if (node == TNULL || key == node->data || isNodePtrValid(node) == false) {
        return node;
      }

      if (key < node->data) {
        return searchTreeHelper(node->left, key);
      }
      return searchTreeHelper(node->right, key);
    }

    // For balancing the tree after deletion
    void deleteFix(NodePtr x) {
      NodePtr s;
      while (x != root && x->color == 0) {
        if (x == x->parent->left) {
          s = x->parent->right;
          if (s->color == 1) {
            s->color = 0;
            x->parent->color = 1;
            leftRotate(x->parent);
            s = x->parent->right;
          }

          if (s->left->color == 0 && s->right->color == 0) {
            s->color = 1;
            x = x->parent;
          } else {
            if (s->right->color == 0) {
              s->left->color = 0;
              s->color = 1;
              rightRotate(s);
              s = x->parent->right;
            }

            s->color = x->parent->color;
            x->parent->color = 0;
            s->right->color = 0;
            leftRotate(x->parent);
            x = root;
          }
        } else {
          s = x->parent->left;
          if (s->color == 1) {
            s->color = 0;
            x->parent->color = 1;
            rightRotate(x->parent);
            s = x->parent->left;
          }

          if (s->right->color == 0 && s->right->color == 0) {
            s->color = 1;
            x = x->parent;
          } else {
            if (s->left->color == 0) {
              s->right->color = 0;
              s->color = 1;
              leftRotate(s);
              s = x->parent->left;
            }

            s->color = x->parent->color;
            x->parent->color = 0;
            s->left->color = 0;
            rightRotate(x->parent);
            x = root;
          }
        }
      }
      x->color = 0;
    }

    void rbTransplant(NodePtr u, NodePtr v) {
      if (u->parent == nullptr) {
        root = v;
      } else if (u == u->parent->left) {
        u->parent->left = v;
      } else {
        u->parent->right = v;
      }
      v->parent = u->parent;
    }

    void deleteNodeHelper(NodePtr node, int key) {
      NodePtr z = TNULL;
      NodePtr x, y;
      while (node != TNULL) {
        if (node->data == key) {
          z = node;
        }

        if (node->data <= key) {
          node = node->right;
        } else {
          node = node->left;
        }
      }

      if (z == TNULL) {
        cout << "Key " << key << " not found in the tree" << endl;
        return;
      }

      y = z;
      int y_original_color = y->color;
      if (z->left == TNULL) {
        x = z->right;
        rbTransplant(z, z->right);
      } else if (z->right == TNULL) {
        x = z->left;
        rbTransplant(z, z->left);
      } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
          x->parent = y;
        } else {
          rbTransplant(y, y->right);
          y->right = z->right;
          y->right->parent = y;
        }

        rbTransplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
      }
      delete z;
      if (y_original_color == 0) {
        deleteFix(x);
      }
    }

    // For balancing the tree after insertion
    void insertFix(NodePtr k) {
      NodePtr u;
      while (k->parent->color == 1) {
        if (k->parent == k->parent->parent->right) {
          u = k->parent->parent->left;
          if (u->color == 1) {
            u->color = 0;
            k->parent->color = 0;
            k->parent->parent->color = 1;
            k = k->parent->parent;
          } else {
            if (k == k->parent->left) {
              k = k->parent;
              rightRotate(k);
            }
            k->parent->color = 0;
            k->parent->parent->color = 1;
            leftRotate(k->parent->parent);
          }
        } else {
          u = k->parent->parent->right;

          if (u->color == 1) {
            u->color = 0;
            k->parent->color = 0;
            k->parent->parent->color = 1;
            k = k->parent->parent;
          } else {
            if (k == k->parent->right) {
              k = k->parent;
              leftRotate(k);
            }
            k->parent->color = 0;
            k->parent->parent->color = 1;
            rightRotate(k->parent->parent);
          }
        }
        if (k == root) {
          break;
        }
      }
      root->color = 0;
    }

    void printHelper(NodePtr root, string indent, bool last) {
      if (isNodePtrValid(root) && root != TNULL) {
        cout << indent;
        if (last) {
          cout << "R----";
          indent += "   ";
        } else {
          cout << "L----";
          indent += "|  ";
        }
        string sColor = root->color ? "RED" : "BLACK";
        cout << root->data << "(" << sColor << ")" << endl;
        printHelper(root->left, indent, false);
        printHelper(root->right, indent, true);
      }
    }

  public:
    int version;
    RedBlackTree *previousTree;
    
    RedBlackTree() {
      TNULL = new Node;
      TNULL->color = 0;
      TNULL->left = nullptr;
      TNULL->right = nullptr;
      root = TNULL;
      version = 0;
      previousTree = nullptr;
    }
    RedBlackTree(const RedBlackTree* oldTree) {
      root = deepCopyNode(oldTree->root);
      TNULL = deepCopyNode(oldTree->TNULL);;
      version = oldTree->version;
      previousTree = oldTree->previousTree;
     }
    RedBlackTree operator=(const RedBlackTree& rbt) {
      RedBlackTree tree2(rbt);
      return tree2;
    };

    RedBlackTree* PointerClone() {
      RedBlackTree* treeClone = new RedBlackTree();

      treeClone->root = deepCopyNode(root);
      treeClone->TNULL = deepCopyNode(TNULL);
      treeClone->version = version;
      if (previousTree != nullptr) {
        RedBlackTree* previousTreeClone = new RedBlackTree(previousTree); 
        treeClone->previousTree = previousTreeClone;
      } else {
        treeClone->previousTree = previousTree;
      }
      return treeClone; 
    }

    void initializeNULLNode(NodePtr node, NodePtr parent) {
      node->data = 0;
      node->parent = parent;
      node->left = nullptr;
      node->right = nullptr;
      node->color = 0;
    }

    void preorder() {
      preOrderHelper(this->root);
    }

    void inorderByVersion(int version) {
      RedBlackTree* tree = getTreeByVersion(version);
      tree->inorder();
    }

    void inorder() {
      inOrderHelper(this->root, 0);
      cout << endl;
    }

    void postorder() {
      postOrderHelper(this->root);
    }

    NodePtr searchTree(int k) {
      return searchTreeHelper(this->root, k);
    }

    NodePtr minimum(NodePtr node) {
      if (isNodePtrValid(node)) {
        while (node->left != TNULL && isNodePtrValid(node->left)) {
          node = node->left;
        }
      }
      return node;
    }

    NodePtr maximum(NodePtr node) {
      while (node->right != TNULL) {
        node = node->right;
      }
      return node;
    }

    NodePtr successor(int key, int version) {
      RedBlackTree* tree = getTreeByVersion(version);
      NodePtr x = tree->searchTree(key);
      if (x == TNULL) {
        return sucessorHelper(tree->getRoot(), key);
      }
      if (x->right != TNULL) {
        return minimum(x->right);
      }

      NodePtr y = x->parent;
      while (y != TNULL && x == y->right) {
        x = y;
        y = y->parent;
      }
      return y;
    }

    NodePtr predecessor(int key) {
      NodePtr x = searchTree(key);
      if (x->left != TNULL) {
        return maximum(x->left);
      }

      NodePtr y = x->parent;
      while (y != TNULL && x == y->left) {
        x = y;
        y = y->parent;
      }

      return y;
    }

    void leftRotate(NodePtr x) {
      NodePtr y = x->right;
      x->right = y->left;
      if (y->left != TNULL) {
        y->left->parent = x;
      }
      y->parent = x->parent;
      if (x->parent == nullptr) {
        this->root = y;
      } else if (x == x->parent->left) {
        x->parent->left = y;
      } else {
        x->parent->right = y;
      }
      y->left = x;
      x->parent = y;
    }

    void rightRotate(NodePtr x) {
      NodePtr y = x->left;
      x->left = y->right;
      if (y->right != TNULL) {
        y->right->parent = x;
      }
      y->parent = x->parent;
      if (x->parent == nullptr) {
        this->root = y;
      } else if (x == x->parent->right) {
        x->parent->right = y;
      } else {
        x->parent->left = y;
      }
      y->right = x;
      x->parent = y;
    }

    // Inserting a node
    void insert(int key) {
      RedBlackTree* oldTree = this->PointerClone();
      this->previousTree = oldTree;

      this->version++;
      NodePtr node = new Node;
      node->parent = nullptr;
      node->data = key;
      node->left = TNULL;
      node->right = TNULL;
      node->color = 1;
      
      NodePtr y = nullptr;
      NodePtr x = this->root;

      while (isNodePtrValid(x) && x != TNULL) {
        y = x;
        if (node->data < x->data) {
          x = x->left;
        } else {
          x = x->right;
        }
      }

      node->parent = y;
      if (y == nullptr) {
        root = node;
      } else if (node->data < y->data) {
        y->left = node;
      } else {
        y->right = node;
      }

      if (node->parent == nullptr) {
        node->color = 0;
        return;
      }

      if (node->parent->parent == nullptr) {
        return;
      }    
      insertFix(node);
    }

    NodePtr getRoot() {
      return this->root;
    }

    void deleteNode(int data) {
      RedBlackTree* oldTree = this->PointerClone();
      this->previousTree = oldTree;
      this->version++;
      deleteNodeHelper(this->root, data);
    }

    void printTree() {
      cout << "Printing tree version " << this->version << endl;
      if (root) {
        printHelper(this->root, "", true);
      }
    }
};

int main() {
  RedBlackTree bst;

  std::string fileContent;
  std::ifstream entryFile("treeExample.txt");
  
  if (entryFile.is_open()) {
    std::string line;
    while (entryFile.good()) {
      std::getline(entryFile, line);
      std::string command = line.substr(0, line.find(" "));
      if (command == "INC") {
        std::string value = line.substr(line.find(" "), line.find("\n"));
        std::cout << "INC" << value << std::endl;
        bst.insert(stoi(value));
      } else if (command == "REM") {
        std::string value = line.substr(line.find(" "), line.find("\n"));
        std::cout << "REM" << value << std::endl;
        bst.deleteNode(stoi(value));
      } else if (command == "SUC") {
        std::string data = line.substr(line.find(" "), line.find("\n"));
        std::string valueToFindSucessor = data.substr(0, line.find(" "));
        std::string version = data.substr(line.find(" "), line.find("\n"));
        NodePtr node = bst.successor(stoi(valueToFindSucessor), stoi(version));
        std::cout << "SUC" << valueToFindSucessor << version << std::endl;
        std::cout << node->data << std::endl;
      } else if (command == "IMP") {
        std::string version = line.substr(line.find(" "), line.find("\n"));
        std::cout << "IMP" << version  << std::endl;
        bst.inorderByVersion(stoi(version));
      }
    }
  } else {
    std::cout << "Couldn't open file\n";
  }

  return 0;  
}