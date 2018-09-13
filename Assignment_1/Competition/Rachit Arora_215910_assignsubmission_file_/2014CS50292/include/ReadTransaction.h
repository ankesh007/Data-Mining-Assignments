#ifndef READ_TRANSACTION_H
#define READ_TRANSACTION_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
typedef pair< vector<int>, int> candidate;
class ReadTransaction
{
private:
    ifstream m_db;
    string m_filepath;

    void openFile();
    void openFile(string filepath);
    void closeFile();

public:
    ReadTransaction(string filepath);
    ~ReadTransaction();
    void resetHead();
    size_t nextTransaction(vector<int>& next);
};

class WriteFrequentItems {
private:
    ofstream m_outfile;
public:
    WriteFrequentItems(string filepath);
    ~WriteFrequentItems();
    void printItems(vector<vector<int> > &freq_items);
    void printItems(vector<candidate> &freq_items);
};


/////////////////////////////////////////////////////////////////////
// Implementation
// ReadTransaction
inline void ReadTransaction::openFile()
{
    openFile(m_filepath);
}

inline void ReadTransaction::openFile(string filepath)
{
    m_db.open(filepath);
    if (!m_db)
    {
        cerr<< "[ERROR] ReadTransaction() : Input file " + filepath + " could not be opened!\n";
        exit(1);
    }
    m_filepath = filepath;
}

inline void ReadTransaction::closeFile()
{
    m_db.close();
}

inline ReadTransaction::ReadTransaction(string filepath)
{
    openFile(filepath);
}

inline ReadTransaction::~ReadTransaction()
{
    closeFile();
}

inline void ReadTransaction::resetHead()
{
    closeFile();
    openFile();
}

inline size_t ReadTransaction::nextTransaction(vector<int>& next)
{
    string next_line;
    if(getline(m_db, next_line).eof()) // Read is complete
    {
        return 0;
    }
    else // next_line should be processed as vector<int> next
    {
        size_t last_position = 0;
        size_t current_position = next_line.find(' ');
        next.clear(); // Clear the parsed vector before processing

        while (current_position != string::npos)
        {
            next.push_back(stoi(next_line.substr(last_position, current_position-last_position)));
            last_position = current_position + 1;
            current_position = next_line.find(' ', last_position);
        }
        if (last_position < next_line.size())
        {
            next.push_back(stoi(next_line.substr(last_position)));
        }

        return next_line.size();
    }
}

/////////////////////////////////////////////////////////////////////
// Implementation
// WriteFrequentItems
inline WriteFrequentItems::WriteFrequentItems(string filepath)
{
    m_outfile.open(filepath);
    if (!m_outfile)
    {
        cerr<< "[ERROR] WriteFrequentItems() : Output file " + filepath + " could not be opened!\n";
        exit(1);
    }
}

inline WriteFrequentItems::~WriteFrequentItems()
{
    m_outfile.close();
}

inline void WriteFrequentItems::printItems(vector<vector<int> > &freq_items)
{
    for(auto item : freq_items)
    {
        for(int i = 0 ; i<item.size()-1; i++)
        {
            m_outfile<<item[i]<<' ';
        }
        m_outfile<<item[item.size()-1]<<endl;
    }
}

inline void WriteFrequentItems::printItems(vector<candidate> &freq_items)
{
    for(auto item : freq_items)
    {
        for(int i = 0 ; i<(item.first).size()-1; i++)
        {
            m_outfile<<(item.first)[i]<<' ';
        }
        m_outfile<<(item.first)[(item.first).size()-1]<<endl;
    }
}
#endif
