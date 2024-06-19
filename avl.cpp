#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Data {
    char staffName[51];
    int expYear;
    Data* left;
    Data* right;
    int height;
};

int maxValue(int a, int b) {
    return (a > b) ? a : b;
}

int height(Data* temp) {
    return (temp == NULL) ? 0 : temp->height;
}

int getBalance(Data* temp) {
    return (temp == NULL) ? 0 : height(temp->left) - height(temp->right);
}

Data* createData(char staffName[], int expYear) {
    Data* newData = (Data*)malloc(sizeof(Data));
    strcpy(newData->staffName, staffName);
    newData->expYear = expYear;
    newData->left = newData->right = NULL;
    newData->height = 1;
    return newData;
}

Data* rightRotate(Data* y) {
    Data* x = y->left;
    Data* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = maxValue(height(y->left), height(y->right)) + 1;
    x->height = maxValue(height(x->left), height(x->right)) + 1;

    return x;
}

Data* leftRotate(Data* x) {
    Data* y = x->right;
    Data* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = maxValue(height(x->left), height(x->right)) + 1;
    y->height = maxValue(height(y->left), height(y->right)) + 1;

    return y;
}

Data* insertData(Data* temp, char staffName[], int expYear) {
    if (temp == NULL) return createData(staffName, expYear);

    if (expYear < temp->expYear) {
        temp->left = insertData(temp->left, staffName, expYear);
    } else if (expYear > temp->expYear) {
        temp->right = insertData(temp->right, staffName, expYear);
    } else {
        return temp;
    }

    temp->height = maxValue(height(temp->left), height(temp->right)) + 1;
    int balance = getBalance(temp);

    if (balance > 1 && expYear < temp->left->expYear) {
        return rightRotate(temp);
    }
    if (balance < -1 && expYear > temp->right->expYear) {
        return leftRotate(temp);
    }
    if (balance > 1 && expYear > temp->left->expYear) {
        temp->left = leftRotate(temp->left);
        return rightRotate(temp);
    }
    if (balance < -1 && expYear < temp->right->expYear) {
        temp->right = rightRotate(temp->right);
        return leftRotate(temp);
    }

    return temp;
}

Data* minValue(Data* temp) {
    Data* curr = temp;
    while (curr->left != NULL) {
        curr = curr->left;
    }
    return curr;
}

Data* deleteData(Data* temp, char staffName[], int expYear) {
    if (temp == NULL) return temp;

    if (expYear < temp->expYear) {
        temp->left = deleteData(temp->left, staffName, expYear);
    } else if (expYear > temp->expYear) {
        temp->right = deleteData(temp->right, staffName, expYear);
    } else {
        if (temp->left == NULL || temp->right == NULL) {
            Data* tempChild = temp->left ? temp->left : temp->right;

            if (tempChild == NULL) {
                tempChild = temp;
                temp = NULL;
            } else {
                *temp = *tempChild;
            }
            free(tempChild);
        } else {
            Data* tempMin = minValue(temp->right);
            temp->expYear = tempMin->expYear;
            strcpy(temp->staffName, tempMin->staffName);
            temp->right = deleteData(temp->right, tempMin->staffName, tempMin->expYear);
        }
    }

    if (temp == NULL) return temp;

    temp->height = maxValue(height(temp->left), height(temp->right)) + 1;
    int balance = getBalance(temp);

    if (balance > 1 && getBalance(temp->left) >= 0) {
        return rightRotate(temp);
    }
    if (balance > 1 && getBalance(temp->left) < 0) {
        temp->left = leftRotate(temp->left);
        return rightRotate(temp);
    }
    if (balance < -1 && getBalance(temp->right) <= 0) {
        return leftRotate(temp);
    }
    if (balance < -1 && getBalance(temp->right) > 0) {
        temp->right = rightRotate(temp->right);
        return leftRotate(temp);
    }

    return temp;
}

Data* findNodeByName(Data* root, char staffName[]) {
    if (root == NULL) return NULL;
    
    if (strcmp(root->staffName, staffName) == 0) {
        return root;
    }

    Data* leftSearch = findNodeByName(root->left, staffName);
    if (leftSearch != NULL) return leftSearch;

    return findNodeByName(root->right, staffName);
}

Data* deleteDataByName(Data* root, char staffName[]) {
    Data* targetNode = findNodeByName(root, staffName);
    if (targetNode != NULL) {
        root = deleteData(root, targetNode->staffName, targetNode->expYear);
    }
    return root;
}

void inOrderWithLabels(Data* cur, Data* root) {
    if(cur){
        inOrderWithLabels(cur->left, root);

        if(cur->height == root->height){
			printf("Leader : %d %s\n", cur->expYear, cur->staffName);
		}else if(cur->height == root->height - 1 && cur == root->left){
			printf("Deputy head 1 : %d %s\n", cur->expYear, cur->staffName);
		}else if(cur->height == root->height - 1 && cur == root->right){
			printf("Deputy head 2 : %d %s\n", cur->expYear, cur->staffName);
		}else if(cur->height > 1 && cur->height < root->height - 1){
			printf("Officer : %d %s\n", cur->expYear, cur->staffName);
		}else if(cur->height == 1){
			printf("Staff : %d %s\n", cur->expYear, cur->staffName);
		}


        inOrderWithLabels(cur->left, root);
    }
}

int main() {
    Data* root = NULL;
    root = insertData(root, "Sasuke", 5);
    root = insertData(root, "Naruto", 6);
    root = insertData(root, "Sakura", 4);
    root = insertData(root, "Madara", 14);
    root = insertData(root, "Orochimaru", 20);
    root = insertData(root, "Kakashi", 15);
    root = insertData(root, "Tsunade", 19);
    root = insertData(root, "Miruzen", 2);
    root = insertData(root, "Jiraya", 24);
    root = insertData(root, "Lee", 3);
    root = deleteDataByName(root, "Madara");
    inOrderWithLabels(root, root);

    return 0;
}
