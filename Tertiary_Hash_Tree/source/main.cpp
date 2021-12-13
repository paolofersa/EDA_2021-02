#include <iostream>
#include <vector>
#include <limits.h>
#include <math.h>
#include <string>
#include <bitset>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iterator>
#include <iomanip>
#include <utility>
#include <chrono>
using namespace std;

//structure for each image
template <class T>
struct ImageStructure
{
    T content;
    string adress;
    ImageStructure(T c, string a) {
        content = c;
        adress = a;
    };
};

//tree node
template <class T>
struct CNode
{
    CNode<T>* children[2];
    string hashAdress;
    vector<ImageStructure<T>> imageContent;
    float distance, maxDistance;
    CNode<T>() {
        children[0] = children[1] = 0;
    };
    CNode<T>(string adress) {
        hashAdress = adress;
        children[0] = children[1] = 0;
    };
    CNode<T>(string adress, ImageStructure<T> content) {
        hashAdress = adress;
        imageContent.push_back(content);
        children[0] = children[1] = 0;
    };
    CNode<T>(string adress, vector<ImageStructure<T>> contentVector) {
        hashAdress = adress;
        imageContent = contentVector;
        children[0] = children[1] = 0;
    };
};

template <class T>
class TertiaryHashTree
{
public:
	TertiaryHashTree();
	~TertiaryHashTree();
    void insert(ImageStructure<T> element);
    void print(CNode<T>*& nodePtr);
    vector<ImageStructure<T>> kNnSearch(ImageStructure<T> elementSearch, int k);
    CNode<T>* root;
private:
    bool findNode(ImageStructure<T> element, CNode<T>** &nodePtr, int& vectorPosition);
    void splitLeafNode(ImageStructure<T> element, CNode<T>**& nodePtr, int& vectorPosition);
    void splitIntermediateNode(ImageStructure<T> element, CNode<T>**& nodePtr, int& vectorPosition);
    void deleteNodes(CNode<T>*& nodePtr);
    float distanceForNode(CNode<T>* &nodePtr, CNode<T>*& parentNode, ImageStructure<T> element, int& vectorPosition);
};

template <class T>
TertiaryHashTree<T>::TertiaryHashTree()
{
    root = new CNode<T>;
}

template <class T>
void TertiaryHashTree<T>::deleteNodes(CNode<T>*& nodePtr) {
    CNode<T>* leftChild = nodePtr->children[0];
    CNode<T>* rightChild = nodePtr->children[1];
    if (leftChild)
        deleteNodes(leftChild);
    if (rightChild)
        deleteNodes(rightChild);
    //if (nodePtr && nodePtr != root)
        //cout << "Node\t" << nodePtr->hashAdress << "\t was deleted\n";
    delete nodePtr;
}

template <class T>
TertiaryHashTree<T>::~TertiaryHashTree()
{
    deleteNodes(root);
}

template <class T>
void TertiaryHashTree<T>::splitLeafNode(ImageStructure<T> element, CNode<T>**& nodePtr, int& vectorPosition) {
    //information for node
    string adressForSplitNode;
    int hashIndex;
    //compare each dimention of the vector
    for (hashIndex = 0; (*nodePtr)->hashAdress[hashIndex] == element.adress[vectorPosition]; hashIndex++, vectorPosition++)
        adressForSplitNode.push_back((*nodePtr)->hashAdress[hashIndex]);
    //create nodes for the split
    (*nodePtr)->children[(int)(*nodePtr)->hashAdress[hashIndex] - 48] = new CNode<T>((*nodePtr)->hashAdress.substr(hashIndex),
                                                                                (*nodePtr)->imageContent);
    (*nodePtr)->children[(int)element.adress[vectorPosition] - 48] = new CNode<T>(element.adress.substr(vectorPosition),
                                                                             element);
    //update values of split node
    (*nodePtr)->imageContent.clear();
    (*nodePtr)->hashAdress = (*nodePtr)->hashAdress.substr(0, hashIndex);

}

template <class T>
void TertiaryHashTree<T>::splitIntermediateNode(ImageStructure<T> element, CNode<T>**& nodePtr, int& vectorPosition) {
    //information for node
    string adressForSplitNode;
    int hashIndex;
    //compare each dimention of the vector
    for (hashIndex = 0; (*nodePtr)->hashAdress[hashIndex] == element.adress[vectorPosition]; hashIndex++, vectorPosition++)
        adressForSplitNode.push_back((*nodePtr)->hashAdress[hashIndex]);
    //create and update nodes for the split
    CNode<T>* newIntermediateNode = new CNode<T>((*nodePtr)->hashAdress.substr(0, hashIndex));
    newIntermediateNode->children[(int)element.adress[vectorPosition] - 48] = new CNode<T>(element.adress.substr(vectorPosition),
                                                                                      element);
    newIntermediateNode->children[(int)(*nodePtr)->hashAdress[hashIndex] - 48] = *nodePtr;
    //update values of split node
    (*nodePtr)->hashAdress = (*nodePtr)->hashAdress.substr(hashIndex);
    *nodePtr = newIntermediateNode;
}

template <class T>
bool TertiaryHashTree<T>::findNode(ImageStructure<T> element, CNode<T>**& nodePtr, int& vectorPosition)
{
    for (nodePtr = &root;
        *nodePtr;
        nodePtr = &((*nodePtr)->children[(int)element.adress[vectorPosition] - 48])) {
         //need split or has the same hashAdress (except root node)
        if (*nodePtr != root && !((*nodePtr)->children[0])) {
            int elementIndex = vectorPosition;
            for (int hashIndex = 0;
                hashIndex < (*nodePtr)->hashAdress.size();
                hashIndex++, elementIndex++)
            {
                //split a leaf node
                if ((*nodePtr)->hashAdress[hashIndex] != element.adress[elementIndex]) {
                    splitLeafNode(element, nodePtr, vectorPosition);
                    return true;
                }
            }
            //add the element in the existing node
            (*nodePtr)->imageContent.push_back(element);
            return true;
        }
        //update value of vectorPosition
        if (*nodePtr == root)
            continue;
        int elementIndex = vectorPosition;
        for (int hashIndex = 0;
            hashIndex < (*nodePtr)->hashAdress.size();
            hashIndex++, elementIndex++)
        {
            //split an intermediate node if not all the adress is the same
            if ((*nodePtr)->hashAdress[hashIndex] != element.adress[elementIndex]) {
                splitIntermediateNode(element, nodePtr, vectorPosition);
                return true;
            }
        }
        vectorPosition = elementIndex;

    }
    return false;
}

template <class T>
void TertiaryHashTree<T>::insert(ImageStructure<T> element)
{
    CNode<T>** nodePtr;
    int vectorPosition = 0;
    if (findNode(element, nodePtr, vectorPosition)) {
        //if the node already exists
        return;
    }
    *nodePtr = new CNode<T>(element.adress.substr(vectorPosition), element);
    
}

template <class T>
void TertiaryHashTree<T>::print(CNode<T>*& nodePtr) {
    CNode<T>* leftChild = nodePtr->children[0];
    CNode<T>* rightChild = nodePtr->children[1];
    if (nodePtr && nodePtr != root)
        cout << "Node\t" << nodePtr->hashAdress << '\n';
    if (leftChild)
        print(leftChild);
    if (rightChild)
        print(rightChild);
}

template <class T>
float TertiaryHashTree<T>::distanceForNode(CNode<T>*& nodePtr, CNode<T>*& parentNode, ImageStructure<T> element, int& vectorPosition) {
    float distance = 0;
    if (parentNode != root)
        distance += parentNode->distance;
    for (int hashIndex = 0; hashIndex < nodePtr->hashAdress.size(); hashIndex++, vectorPosition++) {
        if (nodePtr->hashAdress[hashIndex] != element.adress[vectorPosition])
            distance++;
    }
    return distance;
}

template <class T>
void swapNodes(CNode<T>** a, CNode<T>** b) {
    CNode<T>* temp = *a;
    *a = *b;
    *b = temp;
};

template <class T>
vector<ImageStructure<T>> TertiaryHashTree<T>::kNnSearch(ImageStructure<T> elementSearch, int k) {
    vector<ImageStructure<T>> answer;
    vector<CNode<T>*> stackOfNodes;         //for walk through tree
    vector<CNode<T>*> nodesToCompare;       //for store possible knn nodes
    int vectorPosition = 0, selectedNode;   //auxiliary variables
    CNode<T>* currentNode = root;
    CNode<T>* previousNode = NULL;
    //POSTORDER LOGIC (ITERATIVE using an stack)
    while (currentNode || !stackOfNodes.empty())
    {
        if (currentNode) {
            //calculate the distance except for the root node
            if (currentNode != root) {
                currentNode->distance = distanceForNode(currentNode, stackOfNodes.back(), elementSearch, vectorPosition);
            }
            stackOfNodes.push_back(currentNode);
            //change currentNode to the same next vector according to the elementSearch
            if (vectorPosition < elementSearch.adress.size()) {
                selectedNode = (int)elementSearch.adress[vectorPosition] - 48;
                currentNode = currentNode->children[selectedNode];
            }
            else
                currentNode = NULL;
        }
        else {
            currentNode = stackOfNodes.back();
            //add a node to compare if it is a leaf node
            if (currentNode && currentNode->children[0] == 0)
                nodesToCompare.push_back(currentNode);
            //ask for the other node (with no coincidence) and if it was already visited
            selectedNode = (vectorPosition >= elementSearch.adress.size() || elementSearch.adress[vectorPosition] == '0') ? 1 : 0;
            if (vectorPosition >= elementSearch.adress.size() || currentNode->children[selectedNode] == 0 || currentNode->children[selectedNode] == previousNode) {
                //calculate maximum distance for the node according to its children
                currentNode->maxDistance = (currentNode->children[0] != 0) ?
                                            max(currentNode->children[0]->maxDistance, currentNode->children[1]->maxDistance) :
                                            currentNode->distance;
                stackOfNodes.pop_back();
                vectorPosition = vectorPosition - currentNode->hashAdress.size();
                //stop condition in case is not necesary to go trough all the tree
                float possibleDistance = stackOfNodes.back()->distance + 1 + ((elementSearch.adress.size() - vectorPosition) / 2);
                if (possibleDistance < currentNode->maxDistance || nodesToCompare.size() < k) {
                    //store the previous node and update the current one
                    previousNode = currentNode;
                    currentNode = NULL;
                }
                else {
                    //sort the vector nodesToCompare
                    for (size_t i = 0; i < nodesToCompare.size() - 1; i++){
                        for (size_t j = 0; j < nodesToCompare.size() - i - 1; j++){
                            if (nodesToCompare[j]->distance > nodesToCompare[j + 1]->distance)
                                swapNodes(&nodesToCompare[j], &nodesToCompare[j + 1]);
                        }
                    }                    
                    for (int nodeIndex = 0, imagesInserted = 0; nodeIndex < k; nodeIndex++) {
                        for(int imageIndex = 0; imagesInserted < k && imageIndex < nodesToCompare[nodeIndex]->imageContent.size(); imageIndex++, imagesInserted++)
                            answer.push_back(nodesToCompare[nodeIndex]->imageContent[imageIndex]);
                    }
                    break;
                }               
            }
            else{
                currentNode = currentNode->children[selectedNode];
            }
        }
    }
    return answer;
}

int main()
{
    //READ DATA
    cout << "------------------------------------------CONSTRUCTION------------------------------------------\n\n";
    TertiaryHashTree<string> THT_for_MNIST;
    string line, word;
    bool first = true;

    chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();

    //for TRAINING set (42 000)
    fstream trainingFile("../resources/train.csv", ios::in);
    int trainingCounter = 1;
    if (trainingFile.is_open()) {
        while (getline(trainingFile, line)) {
            string trainingLabel;
            string hashAdress;
            stringstream str(line);
            while (getline(str, word, ',')) {
                if (first) {
                    trainingLabel = "Training image " + to_string(trainingCounter) + ", with label " + word + ".";
                    first = false;
                }
                else{
                    int num = (stoi(word) < 178) ? 0 : 1;
                    string b = to_string(num);
                    hashAdress.push_back(b[0]);
                }
            }
            ImageStructure<string> element(trainingLabel, hashAdress);
            THT_for_MNIST.insert(element);
            first = true;
            trainingCounter++;
        }
    }
    cout << trainingCounter - 1 << " elements from TRAINING set were inserted to the THT.\n" << endl;
    
    //for TEST set (18 000)
    vector<string> elementsForQuery;
    fstream testFile("../resources/test.csv", ios::in);
    int testCounter = 1;
    if (testFile.is_open()) {
        while (getline(testFile, line)) {
            string testLabel = "Test image " + to_string(testCounter) + ", with NO label.";
            string hashAdress;
            stringstream str(line);
            while (getline(str, word, ',')) {
                int num = (stoi(word) < 178) ? 0 : 1;
                string b = to_string(num);
                hashAdress.push_back(b[0]);
            }
            elementsForQuery.push_back(hashAdress);
            ImageStructure<string> element(testLabel, hashAdress);
            THT_for_MNIST.insert(element);
            testCounter++;
        }
    }
    cout << testCounter - 1 << " elements from TEST set were inserted to the THT.\n" << endl;
    end = chrono::high_resolution_clock::now();
    int64_t duration =
        chrono::duration_cast<chrono::nanoseconds>(end - start)
        .count();

    cout << endl;
    cout << "[TIME]\t Construction's DURATION after inserting: " + to_string(trainingCounter + testCounter - 2) + " elements in THT:\t" + to_string(duration) + " ns\n";


    //QUERIES FOR KNN
    cout << "\n\n------------------------------------------KNN QUERY------------------------------------------\n\n";
    int k, numberTestFile;
    cout << "\n Insert NUMBER OF THE IMAGE from test files for the query (1-28000)\n";
    cin >> numberTestFile;
    numberTestFile--;
    cout << "\n Insert K value for the query\n";
    cin >> k;

    string elementForTheQuery = elementsForQuery[numberTestFile];

    ImageStructure<string> elementSearch("SEARCH", elementForTheQuery);
    //time calculation elements
    chrono::time_point<std::chrono::high_resolution_clock> start2, end2;
    start2 = std::chrono::high_resolution_clock::now();
    //KNN
    vector<ImageStructure<string>> answer = THT_for_MNIST.kNnSearch(elementSearch, k);
    //time calculation elements
    end2 = chrono::high_resolution_clock::now();
    int64_t duration2 =
        chrono::duration_cast<chrono::nanoseconds>(end2 - start2)
        .count();

    cout << endl;
    cout << "[TIME]\t Searching's DURATION for k = " + to_string(k) + ":\t" + to_string(duration2) + " ns\n";
    //show the content of answer vector
    cout << "\n\nThe results are the following:\n";
    for (size_t i = 0; i < answer.size(); i++)
        cout << answer[i].content << '\n';
    cout << endl;
    return 0;
}