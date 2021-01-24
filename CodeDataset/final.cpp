#include<iostream>
#include<bits/stdc++.h>

using namespace std;


class bstNode{

public:
    int data;   //docID
    int freq;   //number of times that the word occurs in this doc..
    bstNode* left, *right;

    bstNode()
    {
        data = 0;
        freq = 1;
        left = NULL; right = NULL;
    }

    bstNode(int val)
    {
        freq = 1;
        data = val;
        left = NULL; right = NULL;
    }

};

class BST{

private:
    bstNode* root;

    bstNode* insertNode(bstNode* root, int val)
    {
        if(root == NULL)
            return new bstNode(val);
        else if(val > root->data)
            root->right = insertNode(root->right,val);
        else
            root->left = insertNode(root->left,val);

        return root;
    }

    void inOrder(bstNode* root)
    {
        if(!root)
            return;
        inOrder(root->left);
        cout << root->data << "|" << root->freq << "  ";
        inOrder(root->right);
    }

    bstNode* search(bstNode* root, int key) 
    { 
        if (root == NULL || root->data == key) 
           return root; 
         
        if (root->data < key) 
           return search(root->right, key); 
       
        return search(root->left, key); 
    } 

    void getTreeNodes(bstNode* root, vector<int>& pList)
    {
        if(!root)
            return;
        getTreeNodes(root->left, pList);
        pList.push_back(root->data);
        //cout << root->data << "|" << root->freq << "  ";
        getTreeNodes(root->right, pList);   
    }

    void getTreeNodeFreq(bstNode* root, vector<int>& pFreq)
    {
        if(!root)
            return;
        getTreeNodeFreq(root->left, pFreq);
        pFreq.push_back(root->freq);
        //cout << root->data << "|" << root->freq << "  ";
        getTreeNodeFreq(root->right, pFreq);
    }

public:
    BST()
    {
        root = new bstNode;
    }

    void insert(int val)
    {
        if(root->data == 0)
            root->data = val;
        else
            root = insertNode(root, val);
    }

    void printTree()
    {
        inOrder(root);
        cout << endl;
    }

    bstNode* find(int docID)
    {
        return search(root,docID);
    }

    void getPostingList(vector<int>& pList)
    {
        //vector<int> pList;
        getTreeNodes(root, pList);
        //return pList;
        return;
    }

    void getPostingFreq(vector<int>& pFreq)
    {
        getTreeNodeFreq(root, pFreq);
        return;
    }


};


class TrieNode {

public:

    TrieNode* next[26];
    bool isWordEnd;
    int docFreq;
    BST* postingList;

    TrieNode()
    {
        isWordEnd = false;
        memset(next,0,sizeof(next));
        docFreq = 0;
    }

    void updateWordFreq(string word, int i)
    {
        if((*postingList).find(i))  //if the posting list already has the current doc, update the wordfreq in that doc
        {
            bstNode* docNode;
            docNode = (*postingList).find(i);
            docNode->freq = docNode->freq+1;
        }
        else    //if the posting list doesn't have the current doc, add that doc to the postinglist..
        {
            (*postingList).insert(i);
            docFreq++;
        }
    }

};

class Trie {
private:
    TrieNode* root;

    void traversePrint(TrieNode* root,char* out,int index)
    {
        if(root!=NULL)
        {
            for(int i=0;i<26;i++)
            {
                if(root->next[i]!=NULL)
                {
                    out[index] = 'a'+i;
                    traversePrint(root->next[i],out,index+1);
                }
            }
            if(root->isWordEnd)
            {
                out[index]='\0';
                cout << out << "|" << root->docFreq << " --->  ";
                (*(root->postingList)).printTree();
            }
        }
    }

public:

    Trie() {
        
        root = new TrieNode;
    }

    ~Trie(){
        clear(root);
    }

    void clear(TrieNode *root){
        for(int i = 0; i < 26; i++){
            if(root->next[i] != nullptr){
                clear(root->next[i]);
            }
        }
        delete root;
    }

    void insert(string word, int docID)
    {
        auto temp = root;
        for(int i=0;i<word.size();i++)
        {
            if(temp->next[word[i]-'a'] == NULL)
                temp->next[word[i]-'a'] = new TrieNode;
            temp = temp->next[word[i]-'a'];
        }
        temp->isWordEnd = true;

        temp->docFreq = 1;
        temp->postingList = new BST;
        (*(temp->postingList)).insert(docID);
    }
    
    /** Returns the leaf node, if it is in the trie. */
    TrieNode* search(string word) {

        auto temp = root;
        for(int i=0;i<word.size();i++)
        {
            if(temp->next[word[i]-'a'] == NULL)
                return NULL;
            temp = temp->next[word[i]-'a'];
        }
        
        if(temp->isWordEnd)
            return temp;
        
        return NULL;
    }

    void printWords()
    {
        char out[100];
        traversePrint(root,out,0);
    }
    
};

Trie trie;

bool invalidChar (char c) ;
bool Isquote(char c);

int main()
{
    set<string> stopWords = {"i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its", "itself", "they", "them", "their", "theirs", "themselves", "what", "which", "who", "whom", "this", "that", "these", "those", "am", "is", "are", "was", "were", "be", "been", "being", "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an", "the", "and", "but", "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with", "about", "against", "between", "into", "through", "during", "before", "after", "above", "below", "to", "from", "up", "down", "in", "out", "on", "off", "over", "under", "again", "further", "then", "once", "here", "there", "when", "where", "why", "how", "all", "any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", "than", "too", "very", "s", "t", "can", "will", "just", "don", "should", "now"};
    int N = 2027; //number of docs in the collection
    for(int i=0;i<=2026;i++)
    {
        string fileName = to_string(i);
        //fileName = fileName+".txt";

        //reading the text file..
        //reading the text file line by line..
        cout << "Reading " << fileName << endl;
        ifstream file(fileName);
        string line;
        while (getline(file, line)) {

            //replacing punctuations and non ASCII characters with space in each line (except ')
            replace_if(line.begin() , line.end() ,  
                [] (const char& c) { return (ispunct(c)&&c!=39 || (c>=48 && c<=57));},' ');

            replace_if(line.begin() , line.end() ,  
                [] (const char& c) { return invalidChar(c) ;},' ');

            //removing ' in each line
            line.erase(remove_if(line.begin(),line.end(), Isquote), line.end());

            //extract words from line
            istringstream ss(line); 
            do { 
                // Read a word 
                string word; 
                ss >> word;

                if(stopWords.find(word) != stopWords.end())
                    continue; 
          
                // Print the read word 
                TrieNode* temp; //temp is the leaf node of the word.. 
                if(word.size() < 2)
                    continue;
                temp = trie.search(word);
                if(temp == NULL)
                    trie.insert(word,i);
                else
                    (*temp).updateWordFreq(word,i);
          
                // While there is more to read 
            } while (ss); 
        }

    }

    //printing the inverted index..
    trie.printWords();

    //search a query
    string query;
    cout << "Enter the query to search for.. " << endl;
    getline(cin, query);
    map<string, int> vectorSpace; //will have the words and an index for each word.. (ith word corresponds to the vectorSpace[i]th dimension in the vector space model)
    map<string, double> QueryTermFreqs;
    string term;

    //extracting terms from query
    istringstream iss(query);
    while(iss){
        iss >> term;
        //cout << term << endl;
        vectorSpace[term] = 0;
        if(QueryTermFreqs.find(term) != QueryTermFreqs.end())
            QueryTermFreqs[term]++;
        else
            QueryTermFreqs[term] = 1;
    }
    QueryTermFreqs[term]--;  //coz, the last term is counted one time extra

    //iterate over the map and set the value of each key
    int ind=0;
    for(auto itr= vectorSpace.begin(); itr!=vectorSpace.end(); itr++)
    {
        itr->second = ind;
        ind++;
    }


    //view the vector space dimensions
    // for(auto itr=vectorSpace.begin(); itr!=vectorSpace.end(); itr++)
    // {
    //     cout << itr->first << " : ";
    //     cout << itr->second << endl;
    // }

    //getting a list of all the possible relavent documents for the given query
    vector<int> pList;  //set of all the docs containing the terms in the given query
    for(auto itr=vectorSpace.begin(); itr!=vectorSpace.end(); itr++)
    {
        
        TrieNode* termLeaf1;
        termLeaf1 = trie.search(itr->first);
        if(termLeaf1)
            (*(termLeaf1->postingList)).getPostingList(pList);
    }

    //removing duplicates from pList by doing set conversion
    set<int> s;
    unsigned size = pList.size();
    for( unsigned i = 0; i < size; ++i ) s.insert( pList[i] );
    pList.assign( s.begin(), s.end() );

    int n=vectorSpace.size(); //number of dimensions while, N is no of docs in the collection
    cout << "The set of relavent docs is " << endl;
    for(auto docID : pList)
        cout << docID << " ";
    cout << endl;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //query side

    //finding tf-raw and tf-weight   
    for(auto itr=QueryTermFreqs.begin(); itr!=QueryTermFreqs.end(); itr++)
    {
        if(itr->second)
            itr->second = 1+log(itr->second);
        //cout << itr->first << " --> " << itr->second << endl;
    }

    //finding df and idf
    map<string, double> QueryIDFs;
    int df;
    for(auto itr=vectorSpace.begin(); itr!=vectorSpace.end(); itr++ )
    {
        term = itr->first;
        if(trie.search(term) == NULL)
            QueryIDFs[term]=0;
        else
        {
            df = (trie.search(term))->docFreq;
            QueryIDFs[term] = log(N/df);
        }
    }

    //finding query term weights
    vector<double>QueryTermWeights;
    for(auto itr=vectorSpace.begin(); itr!=vectorSpace.end(); itr++)
    {
        term = itr->first;
        QueryTermWeights.push_back (QueryTermFreqs[term]*QueryIDFs[term]);
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////
    //documents side
    map<int,int>docMap;
    ind=0;
    for(auto docID : pList)
    {
        docMap[ind]=docID;
        ind++;
    }
    vector< vector<double> > mat( pList.size() , vector<double> (n, 0)); 
    for(auto itr=vectorSpace.begin(); itr!=vectorSpace.end(); itr++)  //filling it column by column
    {
        term = itr->first;
        int j = itr->second;
        if(QueryTermWeights[j] == 0)
        {
            for(int i=0;i<pList.size();i++)
                mat[i][j]=0;
        }
        else
        {
            TrieNode* termLeaf;
            termLeaf = trie.search(term);
            for(int i=0;i<pList.size();i++)
            {
                bstNode * docNode = (*(termLeaf->postingList)).find(docMap[i]);
                if(docNode)
                    mat[i][j] = docNode->freq;
            }

        }
    }

    for(int i=0; i<pList.size(); i++)
        for(int j=0; j<n; j++)
        {
            if(mat[i][j])
                mat[i][j] = 1+log(mat[i][j]);
        }

    // for(int i=0; i<pList.size(); i++)
    // {
    //     for(int j=0; j<n; j++)
    //         cout << mat[i][j] << " ";
    //     cout << endl;
    // }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //computing the doc scores
    int nRel = pList.size();
    // vector<int> docScores(nRel,0);
    // for(int i=0;i<nRel; i++)
    // {
    //     for(int j=0;j<n;j++)
    //     {
    //         docScores[i] += mat[i][j]*QueryTermWeights[j];
    //     }
    // }

    // cout << "The score of relavence of each document is.. " << endl;
    // for(int i=0;i<nRel; i++)
    //     cout << docMap[i] << " --> " << docScores[i] << endl;
    map<double, vector<int> > docScores ;
    double score;
    for(int i=0; i<nRel; i++)
    {
        for(int j=0;j<n;j++)
        {
            score += mat[i][j]*QueryTermWeights[j];
        }

        docScores[score].push_back(docMap[i]);
    }

    //ranked retrieval
    ind=0;
    cout << "The top relavent documents with their scores are.. " << endl;
    cout << "docIDS --> score" << endl;
    for(auto itr=docScores.rbegin(); itr!=docScores.rend(); itr++)
    {
        ind++;
        if(ind>10)
            break;
        for(auto docId : itr->second)
            cout << docId << " ";
        cout << "--> " << itr->first << endl;

    }

    return 0;

}


bool invalidChar (char c) 
{  
    return !(c>=0 && c <128);   
}

bool Isquote(char c)
{
    if(c == 39)
        return true;
    else
        return false;
}


