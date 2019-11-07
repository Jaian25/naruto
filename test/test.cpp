#include <bits/stdc++.h>

using namespace std;

struct scoreData{
    char handle[20];
    int score;
    bool operator < (const scoreData &p) const {
        if(score == p.score) return strcmp(handle, p.handle) < 0;
        return score > p.score;
    }
};

std::set< scoreData > LeaderboardData;

int main() {

    FILE *readF = fopen("leaderboard", "r"), *writeF;
    if(readF == NULL) {
        cout << "No File Found" << endl;
    }
    else {
        scoreData temp;
        int rank;
        while(fscanf(readF, "%d %s %d", &rank, temp.handle, &temp.score) != EOF) LeaderboardData.insert( temp );
        fclose(readF);
        //writeF = fopen("leaderboard", "w");
        temp.score += 190;
        rank = 0;
        LeaderboardData.insert(temp);
        for(auto user : LeaderboardData) {
            // cout << user.handle << endl;
            const int total_width = 30;
            const int s_width = strlen(user.handle);
            const int field_width = (total_width - s_width) / 2 + s_width;
            printf("%d %*s %*s%4d\n",++rank,field_width, user.handle, field_width - s_width + (s_width % 2) , " ", user.score);
            //fprintf(writeF, "%d %*s %d\n",++rank, 5,user.handle, user.score);
        }
    }
}