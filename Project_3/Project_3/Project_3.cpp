#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <algorithm>

using namespace std;

class MovieIMDB {
    string title;
    vector<string> genres;
    int year;
    bool adult; //if a movie is rated R(?)

public:
    MovieIMDB() {
        title = "default";
        genres.push_back("");
        year = 0;
        adult = false;
    }
    MovieIMDB(string _title, vector<string> _genres, int _year, bool _adult) {
        title = _title;
        genres = _genres;
        year = _year;
        adult = _adult;
    }
    string getMovie() {
        return title;
    }
    vector<string> getGenres() {
        return genres;
    }
    int getYear() {
        return year;
    }
    bool isAdult() {
        return adult;
    }
    void printMovieInfo() {
        cout << "Actor-Movie Data:" << endl
            << "\tMovie: " << title << endl;
        cout << "\tGenres: ";
        for (int i = 0; i < genres.size(); i++) {
            cout << genres[i];
            if (i != genres.size() - 1)
                cout << ", ";
        }
        cout << endl;

    }
};

//compares 2 movies and makes similarity rank
float compareMovies(MovieIMDB& movie1, MovieIMDB& movie2) {
    /*
      simRank is found by taking the number of matching actors, if its made within 5 years of each other, and the number of matching genres, and dividing by the number of genres (on the movie with the least listed) plus 2 (1 for actor and 1 for year), then multiplying by the rating (to factor in the rating of the movie)
    */
    float simRank = 0.0;

    string s = "";
    int total = 2 + min(movie1.getGenres().size(), movie2.getGenres().size());
    if (abs(movie1.getYear() - movie2.getYear()) < 10)
        simRank++;
    for (int i = 0; i < movie1.getGenres().size(); i++) {
        for (int j = 0; j < movie2.getGenres().size(); j++) {
            if (movie1.getGenres()[i] == movie2.getGenres()[j])
                simRank++;
        }
    }
    if (movie1.isAdult() == movie2.isAdult())
        simRank++;

    return (simRank / total);
}
//creates unordered_map with all movies and their similarity rank to user inputed movie
void createunordered_map(MovieIMDB& userMovie, MovieIMDB& testMovie, unordered_map<float, MovieIMDB>& theHeap) {
    float simRank = compareMovies(userMovie, testMovie);
    theHeap.emplace(simRank, testMovie);

    /* if (theHeap.size() <= 10) {

     }
     else if (simRank > 0) {
         int count = 0;
         for (auto it = theHeap.begin(); it != theHeap.end(); it++) {
             if (count > 10)
                 break;
             if (simRank > it->first) {
                 theHeap.emplace(simRank, testMovie);
             }
         }
     }*/


}
//reads file and compares to user movie, effecively returns a unordered_map using create unordered_map
void readFile(string fileName, unordered_map<float, MovieIMDB>& theHeap, MovieIMDB& userInput) {
    cout << "Reading file..." << endl;
    ifstream inFile;
    //cout << "test" << endl;
    inFile.open(fileName);
    string line, type, title, year, adult, genre, s; //line variable is used for unwanted info, ie. identifier, end year, length etc
    bool isAdult;
    vector<string> genres;
    getline(inFile, line);
    line = "";

    while (inFile) {
        getline(inFile, line, '	');
        //cout << line << " ";
        line = "";
        getline(inFile, type, '	');
        getline(inFile, title, '	');
        getline(inFile, line, '	');
        line = "";
        getline(inFile, adult, '	');
        getline(inFile, year, '	');
        getline(inFile, line, '	');
        line = "";
        getline(inFile, line, '	');
        line = "";
        getline(inFile, genre);
        size_t pos = 0;
        while ((pos = genre.find(",")) != string::npos) {
            s = genre.substr(0, pos);
            genres.push_back(s);
            genre.erase(0, pos + 1);
        }
        if (adult == "0")
            isAdult = false;
        else
            isAdult = true;


        if (year.size() == 2) {
            year = "0000";
        }

        if (type == "movie") {
            MovieIMDB movie(title, genres, stoi(year), isAdult);
            createunordered_map(userInput, movie, theHeap);
            //movies.push_back(movie);
            /*
            cout << type << ", " << title << ", " << adult << ", " << year << ", ";
            for (int i = 0; i < genres.size(); i++) {
                cout << genres[i] << " ";
            }
            cout << endl;
            */
        }
        genres.clear();
        title = "";
        type = "";
        genre = "";
        year = "";
        adult = "";
    }
}
//prints the top 10 similar movies (assuming unordered_map is sorted)
void printHeap(unordered_map<float, MovieIMDB>& similar) {
    int count = 0;
    for (auto pair : similar) {
        cout << "similarity rank: " << pair.first << ", Movie: " << pair.second.getMovie() << endl;
        count++;
        if (count > 9)
            break;
    }
}
//takes user input and returns a movie object
MovieIMDB userInput() {
    cout << "Movie recommendations! enter the following information: " << endl;
    cout << "Release year: ";
    string year;
    cin >> year;

    cout << "Rated R (type yes or no): ";
    string adult;
    cin >> adult;

    cout << "List the genre(s) in a comma seperated list. i.e. action, horror:" << endl;
    string genre;
    cin >> genre;

    bool isAdult;
    if (adult == "no")
        isAdult = false;
    else
        isAdult = true;

    vector<string> genres;
    size_t pos = 0;
    string s;
    while ((pos = genre.find(",")) != string::npos) {
        s = genre.substr(0, pos);
        genres.push_back(s);
        genre.erase(0, pos + 2);
    }
    MovieIMDB userMovie("user input", genres, stoi(year), isAdult);
    return userMovie;
}

int partition(vector<float>& vec, int left, int right) {
    int pivotIndex = left + (right - left) / 2;
    float pivotValue = vec[pivotIndex];
    int i = left, j = right;
    float temp;
    while (i <= j) {
        while (vec[i] < pivotValue) {
            i++;
        }
        while (vec[j] > pivotValue) {
            j--;
        }
        if (i <= j) {
            temp = vec[i];
            vec[i] = vec[j];
            vec[j] = temp;
            i++;
            j--;
        }
    }
    return i;
}

void quicksort(std::vector<float>& vec, int left, int right) {
    if (left < right) {
        int pivotIndex = partition(vec, left, right);
        quicksort(vec, left, pivotIndex - 1);
        quicksort(vec, pivotIndex, right);
    }
}

void mergesort(std::vector<float>& vec)
{
    // If the vector has less than two elements, it is already sorted.
    if (vec.size() < 2) return;

    // Split the vector in half.
    std::size_t const half = vec.size() / 2;
    std::vector<float> left(vec.begin(), vec.begin() + half);
    std::vector<float> right(vec.begin() + half, vec.end());

    // Sort the two halves recursively.
    mergesort(left);
    mergesort(right);

    // Merge the two sorted halves into the original vector.
    std::merge(left.begin(), left.end(), right.begin(), right.end(), vec.begin());
}

int main(int argc, char** argv) {

    MovieIMDB userMovie = userInput();

    //vector<MovieIMDB> movies;
    // 
    //unordered_map auto sorts and acts as a sort of heap
    unordered_map<float, MovieIMDB> theHeap;
    //    unordered_map<float, MovieIMDB, greater<float>> theHeap;


    readFile("data.tsv", theHeap, userMovie);


    vector<float> quickSortRanks;
    vector<float> mergeSortRanks;

    for (auto it = theHeap.begin(); it != theHeap.end(); it++) {
        quickSortRanks.push_back(it->first);
        mergeSortRanks.push_back(it->first);
    }
    //printHeap(theHeap);

    quicksort(quickSortRanks, 0, quickSortRanks.size() - 1);
    mergesort(mergeSortRanks);

    for (int i = 1; i < 11; i++) {
        cout << i << ". " << theHeap[quickSortRanks[quickSortRanks.size() - i]].getMovie() << ": " << quickSortRanks[quickSortRanks.size() - i] << endl;
    }

    return 0;
}
