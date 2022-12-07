#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "Bridges.h"
#include "DataSource.h"
#include "data_src/ActorMovieIMDB.h"
#include <string>
#include "AVLTreeElement.h"
#include <map>
using namespace std;
using namespace bridges;

class Heap {
    map<string, float> similarList;


};
/*
  Step 1: Get the Bridges USER ID and API KEY for your program
          by creating an account and signing up (You can use any
          email): https://bridges-cs.herokuapp.com/login

  Step 2: User ID (User name) and Key (API Sha1 Key) can be
          found under Profile on the top left
*/
//prints info of the movie
void printMovieInfo(ActorMovieIMDB& movie) {
    cout << "Actor-Movie Data:" << endl
        << "\tActor: " << movie.getActor() << endl
        << "\tMovie: " << movie.getMovie() << endl
        << "\tRating: " << movie.getMovieRating() << endl;
    cout << "\tGenres: ";
    for (int i = 0; i < movie.getGenres().size(); i++) {
        cout << movie.getGenres()[i];
        if (i != movie.getGenres().size() - 1)
            cout << ", ";
    }
    cout << endl;

}
//prints heap/most similar movies in decending order
void printHeap(map<float, string, greater<float>>& similar) {
    int count = 0;
    for (auto pair : similar) {
        cout << "similarity rank: " << pair.first << ", Movie: " << pair.second << endl;
        count++;
        if (count > 9)
            break;
    }

}
//gets year from title
int getYear(ActorMovieIMDB& movie) {
    string s = "";
    for (auto i = movie.getMovie().find('(') + 1; i < movie.getMovie().find(')');i++) {
        s += movie.getMovie()[i];

    }
    return stoi(s);
}
//compares 2 movies and makes similarity rank
float compareMovies(ActorMovieIMDB& movie1, ActorMovieIMDB& movie2) {
    /*
      simRank is found by taking the number of matching actors, if its made within 5 years of each other, and the number of matching genres, and dividing by the number of genres (on the movie with the least listed) plus 2 (1 for actor and 1 for year), then multiplying by the rating (to factor in the rating of the movie)
    */
    float simRank = 0.0;

    string s = "";
    int total = 2 + min(movie1.getGenres().size(), movie2.getGenres().size());
    if (movie1.getActor() == movie2.getActor())
        simRank++;
    if (abs(getYear(movie1) - getYear(movie2)) < 5)
        simRank++;
    for (int i = 0; i < movie1.getGenres().size(); i++) {
        for (int j = 0; j < movie2.getGenres().size(); j++) {
            if (movie1.getGenres()[i] == movie2.getGenres()[j])
                simRank++;
        }
    }
    return (simRank / total) * (float)movie2.getMovieRating();
}
//create heap (sorted map) with movies with a top similarity score
void readHeap(vector<ActorMovieIMDB>& actor_list, map<float, string, greater<float>>& similar, int input) {
    float simRank;
    int count = 0;
    float mean;
    cout << "Movies similar to " << actor_list[input].getMovie() << endl;
    for (int i = 0; i < actor_list.size(); i++) {
        if (i != input) {
            simRank = compareMovies(actor_list[input], actor_list[i]);

        }
        if (simRank > 2) {
            similar.emplace(simRank, actor_list[i].getMovie());
            mean += simRank;
            count++;
        }
    }
    cout << "count: " << count << ", mean: " << mean / count << endl;

}

int main(int argc, char** argv) {

    Bridges bridges(1, "north", "58473698554");
    bridges.setTitle("Accessing IMDB Data");

    // create data source object
    DataSource ds(&bridges);

    // get the actor movie IMDB data through the BRIDGES API
    vector<ActorMovieIMDB> actor_list = ds.getActorMovieIMDBData2();
    map<float, string, greater<float>> similar;
    int input = 222;

    printMovieInfo(actor_list[input]);
    readHeap(actor_list, similar, input);
    printHeap(similar);

    return 0;
}