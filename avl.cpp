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

void inOrderWithLabels(Data* root, int depth, int parentLabel) {
    if (root) {
        inOrderWithLabels(root->left, depth + 1, parentLabel);

        if (depth == 0) {
            printf("Research Leader : %d Years Exp %s\n", root->expYear, root->staffName);
        } else if (depth == 1) {
            if (parentLabel == 0) {
                printf("Deputy Head 1 : %d Years Exp %s\n", root->expYear, root->staffName);
            } else {
                printf("Deputy Head 2 : %d Years Exp %s\n", root->expYear, root->staffName);
            }
        } else if (depth == 2) {
            printf("Officer : %d Years Exp %s\n", root->expYear, root->staffName);
        } else {
            printf("Staff : %d Years Exp %s\n", root->expYear, root->staffName);
        }

        inOrderWithLabels(root->right, depth + 1, depth == 0 ? 1 : parentLabel);
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
    root = deleteData(root, "Sasuke", 5);
    inOrderWithLabels(root, 0, 0);

    return 0;
}
