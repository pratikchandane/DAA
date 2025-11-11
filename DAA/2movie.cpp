#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Movie structure
struct Movie {
    string title;
    float imdb_rating;
    int release_year;
    int watch_time;
};

// Partition function for QuickSort
int partition(vector<Movie>& movies, int low, int high, string parameter) {
    // Pivot element
    Movie pivot = movies[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        bool condition = false;

        // Compare based on parameter selected by user
        if (parameter == "rating") {
            condition = (movies[j].imdb_rating < pivot.imdb_rating);
        } 
        else if (parameter == "year") {
            condition = (movies[j].release_year < pivot.release_year);
        } 
        else if (parameter == "watchtime") {
            condition = (movies[j].watch_time < pivot.watch_time);
        }

        if (condition) {
            i++;
            swap(movies[i], movies[j]);
        }
    }
    swap(movies[i + 1], movies[high]);
    return (i + 1);
}

// QuickSort function
void quickSort(vector<Movie>& movies, int low, int high, string parameter) {
    if (low < high) {
        int pi = partition(movies, low, high, parameter);
        quickSort(movies, low, pi - 1, parameter);
        quickSort(movies, pi + 1, high, parameter);
    }
}

// Display movie list
void displayMovies(const vector<Movie>& movies) {
    cout << "\nSorted Movie List:\n";
    
    for (auto m : movies) {
        cout << "Title: " << m.title 
             << " | Rating: " << m.imdb_rating 
             << " | Year: " << m.release_year 
             << " | Watch Time: " << m.watch_time << " mins\n";
    }
    
}

int main() {
    vector<Movie> movies = {
        {"Inception", 8.8, 2010, 148},
        {"Avatar", 7.8, 2009, 162},
        {"Interstellar", 8.6, 2014, 169},
        {"The Dark Knight", 9.0, 2008, 152},
        {"Tenet", 7.3, 2020, 150}
    };

    string parameter;
    cout << "Enter parameter to sort by (rating / year / watchtime): ";
    cin >> parameter;

    quickSort(movies, 0, movies.size() - 1, parameter);
    displayMovies(movies);

    return 0;
}

