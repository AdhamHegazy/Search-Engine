
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <iostream>
using namespace std;

struct WebPage //a class that holds information for each webpage
{
    string link;
    float impressions;
    float clicks;
    float CTR; // (clicks/impressions);
    float PageRank;
    float score;
    vector<string> Pointing_To; //a vector that holds the websites this website is pointing to
    vector<string> Pointed_From; //a vector that holds the websites that points to this website
    WebPage()
    {
    }

    WebPage(string Link)
    {
        link = Link;
    }
};

unordered_map<string, WebPage> linker;

float number_of_webpages = 0;

void Impressions()
{
    ifstream file;
    file.open("/Users/adham/Desktop/Code/Engine/Impressions.txt"); //change this to the file the impressions.txt is placed at
    string input;

    string secondd;
    string temp;
    string webpage;

    while (getline(file, input))
    {
        input.push_back(',');
        bool bfirst = true;
        for (int i = 0; i < input.length(); i++)
        {
            if (input[i] != ',')
                temp.push_back(input[i]);
            else
            {
                if (bfirst)
                {
                    webpage = temp;
                    linker[webpage] = WebPage(webpage); //building a WebPage class from the link parsed
                    number_of_webpages++;
                    temp.clear();
                }
                else
                {

                    WebPage tempWebPage = linker[webpage];
                    tempWebPage.impressions = stoi(temp);
                    tempWebPage.clicks = 0;
                    linker[webpage] = tempWebPage;
                    temp.clear();
                }
                bfirst = false;
            }
        }
    }
}

void Graphs() //linking the graphs of the websites
{
    ifstream file;
    file.open("/Users/adham/Desktop/Code/Engine/WebGraph.txt"); //change path to where the WebGraph.txt is being placed at
    string input;

    string secondd;
    string temp;
    string webpage;
    while (getline(file, input))
    {
        input.push_back(',');
        bool bfirst = true;
        for (int i = 0; i < input.length(); i++)
        {
            if (input[i] != ',')
                temp.push_back(input[i]);
            else
            {
                if (bfirst)
                {
                    webpage = temp;
                    temp.clear();
                }
                else
                {

                    WebPage tempWebPage = linker[webpage];
                    tempWebPage.Pointing_To.push_back(temp);

                    linker[webpage] = tempWebPage;

                    WebPage tempWebPage1 = linker[temp];
                    if (tempWebPage1.link != temp)
                    {
                        tempWebPage1.link = temp;
                        number_of_webpages++;
                    }
                    tempWebPage1.Pointed_From.push_back(webpage);
                    linker[temp] = tempWebPage1;
                    temp.clear();
                }
                bfirst = false;
            }
        }
        cout << "Pointing to: " << linker[webpage].link << endl;
        for (int i = 0; i < linker[webpage].Pointing_To.size(); i++)
        {
            cout << linker[webpage].Pointing_To.at(i) << endl;
            cout << "Pointed from: \n";
            for (int j = 0; j < linker[linker[webpage].Pointing_To.at(i)].Pointed_From.size(); j++)
            {
                cout << linker[linker[webpage].Pointing_To.at(i)].Pointed_From.at(j) << endl;
            }
        }
    }
}

unordered_map<string, vector<string> > keys;
void Keywords()
{
    ifstream file;
    file.open("/Users/adham/Desktop/Code/Engine/Keywords.txt"); //Change to where keywords.txt is being placed at
    string input;
    string first;
    string second;

    if (!file)
    {
        file.close();
        cout << "Error while openinig " << endl;
        return;
    }

    while (getline(file, input))
    {
        string temp;
        string webpage;
        vector<string> strings;
        vector<string> tempvector;
        bool bfirst = true;
        input.push_back(',');

        for (int i = 0; i < input.length(); i++)
        {
            if (input[i] != ',')
                temp.push_back(input[i]);
            else
            {
                if (bfirst)
                {
                    webpage = temp;
                    temp.clear();
                }
                else
                {

                    tempvector = keys[temp];
                    tempvector.push_back(webpage);
                    keys[temp] = tempvector;

                    strings.push_back(temp);
                    temp.clear();
                }
                bfirst = false;
            }
        }

    }

}


void getNextLineAndSplitIntoTokens(vector<string> &result, istream &str)
{
    string line;
    getline(str, line);

    stringstream lineStream(line);
    string cell;

    while (getline(lineStream, cell, ','))
    {
        result.push_back(cell);
    }

}

vector<WebPage> results;
string first;
string second;
string third;

vector<string> search_query;
void NewSearch() //parsing the newsearch
{
    search_query.clear();
    first = " ";
    second = " ";
    third = " ";
    string input;
    cin.ignore();
    getline(cin, input);
    stringstream ss(input);
    ss >> first >> second >> third;

    if (input[0] == '"')
    {
        search_query.push_back(input.substr(1, input.find_last_of("\"")));
        search_query.back().pop_back();
    }
    else
    {
        if (second == "OR")
        {
            search_query.push_back(input.substr(0, input.find(" ")));
            search_query.push_back("OR");
            search_query.push_back(input.substr(search_query.at(0).length() + 4, input.size() - 1));
        }
        else
        {
            if (second == "AND")
            {
                search_query.push_back(input.substr(0, input.find(" ")));
                search_query.push_back("AND");
                search_query.push_back(input.substr(search_query.at(0).length() + 5, input.size() - 1));
            }
            else
            {
                search_query.push_back(input.substr(0, input.find(" ")));
                search_query.push_back("OR");
                search_query.push_back(input.substr(search_query.at(0).length() + 1, input.size() - 1));
            }
        }
    }
}
bool compWebPages(WebPage One, WebPage Two)
{
    return One.score > Two.score;
}

void PageRank() //calculation of pagerank
{

    for (auto i = linker.begin(); i != linker.end(); ++i)
    {
        i->second.PageRank = 1 / number_of_webpages;
    }

    for (auto k = linker.begin(); k != linker.end(); ++k)
    {
        for (int j = 0; j < k->second.Pointed_From.size(); j++)
        {
            k->second.PageRank += linker[k->second.Pointed_From.at(j)].PageRank / linker[k->second.Pointed_From.at(j)].Pointing_To.size();
        }
    }

    for (auto r = linker.begin(); r != linker.end(); ++r)
    {
        for (int j = 0; j < r->second.Pointed_From.size(); j++)
        {
            r->second.PageRank += linker[r->second.Pointed_From.at(j)].PageRank / linker[r->second.Pointed_From.at(j)].Pointing_To.size();
        }
    }

    int y = 0;
    for (auto h = linker.begin(); h != linker.end(); ++h)
    {
        cout << "Page rank of " << h->second.link << " is " << h->second.PageRank << endl;

        y++;
    }

}

void DisplayResults()
{
    cout << "Results: " << endl;
    int count = 0;
    vector<string> temp1;
    vector<string> temp2;
    cout << "size " << search_query.size() << endl;
    if (search_query.size() == 1)
    {   
        
        for (int i = 0; i < keys[search_query.at(0)].size(); i++)
        {
           
            linker[keys[search_query.at(0)].at(i)].impressions++;
            results.push_back(linker[keys[search_query.at(0)].at(i)]);
            linker[keys[search_query.at(0)].at(i)].score = 0.4 * linker[keys[search_query.at(0)].at(i)].PageRank + ((1 - (0.1 * linker[keys[search_query.at(0)].at(i)].impressions) / (1 + 0.1 * linker[keys[search_query.at(0)].at(i)].impressions)) * linker[keys[search_query.at(0)].at(i)].PageRank + ((0.1 * linker[keys[search_query.at(0)].at(i)].impressions) / (1 + 0.1 * linker[keys[search_query.at(0)].at(i)].impressions)) * linker[keys[search_query.at(0)].at(i)].CTR) * 0.6;
        }
    }
    else
    {
        if (search_query.at(1) == "AND")
        {
            
            for (int i = 0; i < keys[search_query.at(0)].size(); i++)
            {

                temp1.push_back(keys[search_query.at(0)].at(i));
            }
            for (int j = 0; j < keys[search_query.at(2)].size(); j++)
            {

                temp2.push_back(keys[search_query.at(2)].at(j));
            }

            for (int i = 0; i < keys[search_query.at(0)].size(); i++)
            {
                for (int j = 0; j < keys[search_query.at(2)].size(); j++)
                {

                    if (temp1.at(i) == temp2.at(j))
                    {                       
                        results.push_back(linker[temp1.at(i)]);
                        linker[temp1.at(i)].impressions++;
                        linker[temp1.at(i)].score = 0.4 * linker[temp1.at(i)].PageRank + ((1 - (0.1 * linker[temp1.at(i)].impressions) / (1 + 0.1 * linker[temp1.at(i)].impressions)) * linker[temp1.at(i)].PageRank + ((0.1 * linker[temp1.at(i)].impressions) / (1 + 0.1 * linker[temp1.at(i)].impressions)) * linker[temp1.at(i)].CTR) * 0.6;
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < keys[search_query.at(0)].size(); i++)
            {
                count++;
                linker[keys[search_query.at(0)].at(i)].impressions++;
                results.push_back(linker[keys[search_query.at(0)].at(i)]);
                linker[keys[search_query.at(0)].at(i)].score = 0.4 * linker[keys[search_query.at(0)].at(i)].PageRank + ((1 - (0.1 * linker[keys[search_query.at(0)].at(i)].impressions) / (1 + 0.1 * linker[keys[search_query.at(0)].at(i)].impressions)) * linker[keys[search_query.at(0)].at(i)].PageRank + ((0.1 * linker[keys[search_query.at(0)].at(i)].impressions) / (1 + 0.1 * linker[keys[search_query.at(0)].at(i)].impressions)) * linker[keys[search_query.at(0)].at(i)].CTR) * 0.6;
            }
            for (int j = 0; j < keys[search_query.at(2)].size(); j++)
            {
                count++;

                linker[keys[search_query.at(2)].at(j)].impressions++;
                linker[keys[search_query.at(2)].at(j)].score = 0.4 * linker[keys[search_query.at(2)].at(j)].PageRank + ((1 - (0.1 * linker[keys[search_query.at(2)].at(j)].impressions) / (1 + 0.1 * linker[keys[search_query.at(2)].at(j)].impressions)) * linker[keys[search_query.at(2)].at(j)].PageRank + ((0.1 * linker[keys[search_query.at(2)].at(j)].impressions) / (1 + 0.1 * linker[keys[search_query.at(2)].at(j)].impressions)) * linker[keys[search_query.at(2)].at(j)].CTR) * 0.6;

                results.push_back(linker[keys[search_query.at(2)].at(j)]);
            }
        }
    }


    sort(results.begin(), results.end(), compWebPages);
    cout << "Sorted results: \n";
    for (int k = 0; k < results.size(); k++)
    {
        cout << k + 1 << ". " << results.at(k).link << endl;
    }
}

void newsearch()
{

    cout << endl;
    cout << "Please enter your query: ";

    NewSearch();

    DisplayResults();
}

void ChooseWebPage()
{
    int z;
    cout << "Choose one of the results: " << endl;
    cin >> z;
    z = z - 1;
    cout << "You are now viewing " << results.at(z).link << endl;

    linker[results.at(z).link].clicks++;
    linker[results.at(z).link].CTR = linker[results.at(z).link].clicks / linker[results.at(z).link].impressions;

    linker[results.at(z).link].score = 0.4 * linker[results.at(z).link].PageRank + ((1 - (0.1 * linker[results.at(z).link].impressions) / (1 + 0.1 * linker[results.at(z).link].impressions)) * linker[results.at(z).link].PageRank + ((0.1 * linker[results.at(z).link].impressions) / (1 + 0.1 * linker[results.at(z).link].impressions)) * linker[results.at(z).link].CTR) * 0.6;

    cout << "Clicks: " << linker[results.at(z).link].clicks << endl;

    cout << "CTR: " << linker[results.at(z).link].CTR << endl;
}

int main()
{

    Keywords();
    Impressions();
    Graphs();
    PageRank();
    int x;
    bool stop = false;

    while (!stop)
    {
        cout << "What would you like to do ? \n";
        cout << "1. New search \n";
        cout << "2. Exit \n";
        cout << "Type in your choice: ";

        cin >> x;

        if (x == 2)
        {
            stop = true;
        }
        else
        {
            if (x == 1)
            {
                int y = 2;
                do
                {
                    if (y == 2)
                    {
                        results.clear();
                        newsearch();
                    }
                    cout << "What would you like to do ? \n";
                    cout << "1. Choose a webpage to open \n";
                    cout << "2. New search \n";
                    cout << "3. Exit \n";
                    cout << "Type in your choice: ";
                    cin >> y;
                    if (y == 1)
                    {
                        ChooseWebPage();
                        cout << "1. Back to search results \n";
                        cout << "2. New search \n";
                        cout << "3. Exit \n";
                        cin >> y;
                        if (y == 1)
                        {
                            results.clear();
                            DisplayResults();
                            y = 4;
                        }
                    }

                    if (y == 3)
                    {
                        stop = true;
                    }

                } while (y == 2 || y == 4);
            }
        }
    }

}
