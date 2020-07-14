//
//  main.cpp
//  hw2
//
//  Created by 蕭楚澔 on 2020/4/27.
//  Copyright © 2020 Bob. All rights reserved.
//

#include <iostream>
#include <vector>
#include <math.h>
#include <stack>
#include <time.h>

using namespace std;

stack<pair<int, bool>> s;
//決定要使用的 h funtion
bool MRV_true = false;
bool MRV_degree_true = false;
bool MRV_degree_LCV_true = false;

//檢查 mine 總量是否等於輸入的數量
bool mine_equal(vector<int> mine, int m){
    int ans = 0;
    for(int i=0; i<mine.size(); i++){
        ans+=mine[i];
    }
    if(ans == m){
        return true;
    }
    else{
        return false;
    }
}

//用於lcv, 判斷輸入 0,1 後 constraint 大小
int find_constraint(vector<int> mine, vector<bool> hint, vector<int> domain, int place, bool b){
    int ans = 0;
    domain[place] = -1;
    if(b == 0){
        mine[place] = -2;
    }
    else{
        mine[place] = -3;
    }
    int t=sqrt(mine.size());
    vector<int> v;
    int arr[t][t];
    for(int i=0; i<t; i++){
        for(int j=0; j<t; j++){
            arr[i][j] = i*t+j;
        }
    }
    for(int i=0; i<mine.size(); i++){
        v.clear();
        if(hint[i] == true){
            int x=(i/t);
            int y=(i%t);
            if(y-1 >= 0){
                v.push_back(arr[x][y-1]);
            }
            if(y+1 < t){
                v.push_back(arr[x][y+1]);
            }
            if(x-1 >= 0){
                v.push_back(arr[x-1][y]);
            }
            if(x-1 >= 0 && y-1 >= 0){
                v.push_back(arr[x-1][y-1]);
            }
            if(x-1 >= 0 && y+1 < t){
                v.push_back(arr[x-1][y+1]);
            }
            if(x+1 < t){
                v.push_back(arr[x+1][y]);
            }
            if(x+1 < t && y-1 >= 0){
                v.push_back(arr[x+1][y-1]);
            }
            if(x+1 < t && y+1 < t){
                v.push_back(arr[x+1][y+1]);
            }
            int upperbound = 0;
            int lowerbound = 0;
            for(int j=0; j<v.size(); j++){
                if(domain[v[j]] != 3 && domain[v[j]] != -1){
                    upperbound+=1;
                }
                else if(mine[v[j]] == -3){
                    mine[i]--;
                }
            }
            
            if(upperbound == mine[i]){
                for(int k=0; k<v.size(); k++){
                    if(domain[v[k]] == 2){
                        domain[v[k]] = 1;
                    }
                }
            }
            else if(lowerbound == mine[i]){
                for(int k=0; k<v.size(); k++){
                    if(domain[v[k]] == 2){
                        domain[v[k]] = 0;
                    }
                }
            }
        }
    }
    for(int i=0; i<domain.size(); i++){
        if(hint[i] == false && domain[i] == 0){
            ans++;
        }
        else if(hint[i] == false && domain[i] == 1){
            ans++;
        }
        else if(hint[i] == false && domain[i] == 2){
            ans+=2;
        }
    }
    return ans;
}

//用於degree, 如果mrv都找不到 domain=1 的, 就找degree中 constraint 最大的
int find_degree(vector<int> domain, vector<bool> hint, vector<int> degree, vector<bool> &sign){
    for(int i=0; i<domain.size(); i++){
        if(domain[i] == 0){
            return i;
        }
    }
    for(int i=0; i<domain.size(); i++){
        if(domain[i] == 1){
            return i;
        }
    }
    int max = -1;
    int ans = (int)degree.size();
    for(int i=0; i<degree.size(); i++){
        if(domain[i] == 2 && degree[i] > max && sign[i] == false){
            max = degree[i];
            ans = i;
        }
    }
    if(ans<degree.size()){
        sign[ans] = true;
    }
    return ans;
}

//用於mrv, 優先找domain = 1, 如果都沒有就找 domain = 2
int find_domain_1(vector<int> domain){
    for(int i=0; i<domain.size(); i++){
        if(domain[i] == 0){
            return i;
        }
    }
    for(int i=0; i<domain.size(); i++){
        if(domain[i] == 1){
            return i;
        }
    }
    for(int i=0; i<domain.size(); i++){
        if(domain[i] == 2){
            return i;
        }
    }
    return (int)domain.size();
}

//類似 forward checking, 尋找所有 hint 的 upperbound & lowerbound, 接著把所有 domain = 1 的填上應填的數字
void change_domain(vector<int> mine, vector<bool> hint, vector<int> &domain){
    int t=sqrt(mine.size());
    vector<int> v;
    int arr[t][t];
    for(int i=0; i<t; i++){
        for(int j=0; j<t; j++){
            arr[i][j] = i*t+j;
        }
    }
    for(int i=0; i<mine.size(); i++){
        v.clear();
        if(hint[i] == true){
            int x=(i/t);
            int y=(i%t);
            if(y-1 >= 0){
                v.push_back(arr[x][y-1]);
            }
            if(y+1 < t){
                v.push_back(arr[x][y+1]);
            }
            if(x-1 >= 0){
                v.push_back(arr[x-1][y]);
            }
            if(x-1 >= 0 && y-1 >= 0){
                v.push_back(arr[x-1][y-1]);
            }
            if(x-1 >= 0 && y+1 < t){
                v.push_back(arr[x-1][y+1]);
            }
            if(x+1 < t){
                v.push_back(arr[x+1][y]);
            }
            if(x+1 < t && y-1 >= 0){
                v.push_back(arr[x+1][y-1]);
            }
            if(x+1 < t && y+1 < t){
                v.push_back(arr[x+1][y+1]);
            }
            int upperbound = 0;
            int lowerbound = 0;
            for(int j=0; j<v.size(); j++){
                if(domain[v[j]] != 3 && domain[v[j]] != -1){
                    upperbound+=1;
                }
            }
            
            if(upperbound == mine[i]){
                for(int k=0; k<v.size(); k++){
                    if(domain[v[k]] == 2){
                        domain[v[k]] = 1;
                    }
                }
            }
            else if(lowerbound == mine[i]){
                for(int k=0; k<v.size(); k++){
                    if(domain[v[k]] == 2){
                        domain[v[k]] = 0;
                    }
                }

            }
        }
    }
}

//把一維 vector 以二維輸出
void output_board(vector<int> v){
    int t=(int)v.size();
    t = sqrt(t);
    for(int i=0; i<t; i++){
        for(int j=0; j<t; j++){
            cout<<v[t*i+j];
        }
        cout<<endl;
    }
}

//如果有 hint != 0, 代表還沒結束
bool check_hint_zero(vector<int> mine, vector<bool> hint){
    bool ans=true;
    for(int i=0; i<hint.size(); i++){
        if(hint[i] == true && mine[i] != 0){
            ans=false;
            break;
        }
    }
    return ans;
}

//如果有 hint <= 0, 代表不能再放炸彈了
bool check_hint_greater_than_zero(vector<int> mine, vector<bool> hint){
    bool ans=true;
    for(int i=0; i<hint.size(); i++){
        if(hint[i] == true && mine[i] < 0){
            ans=false;
            break;
        }
    }
    return ans;
}

//放炸彈後, 要更改附近的 hint
void change_hint(vector<int> &mine, vector<bool> hint, int p, bool b){
    int t=sqrt(mine.size());
    vector<int> v;
    int arr[t][t];
    for(int i=0; i<t; i++){
        for(int j=0; j<t; j++){
            arr[i][j] = i*t+j;
        }
    }
    int x=(p/t);
    int y=(p%t);
    if(y-1 >= 0){
        v.push_back(arr[x][y-1]);
    }
    if(y+1 < t){
        v.push_back(arr[x][y+1]);
    }
    if(x-1 >= 0){
        v.push_back(arr[x-1][y]);
    }
    if(x-1 >= 0 && y-1 >= 0){
        v.push_back(arr[x-1][y-1]);
    }
    if(x-1 >= 0 && y+1 < t){
        v.push_back(arr[x-1][y+1]);
    }
    if(x+1 < t){
        v.push_back(arr[x+1][y]);
    }
    if(x+1 < t && y-1 >= 0){
        v.push_back(arr[x+1][y-1]);
    }
    if(x+1 < t && y+1 < t){
        v.push_back(arr[x+1][y+1]);
    }
    
    if(b == 1){
        for(int i=0; i<v.size(); i++){
            if(hint[v[i]] == true){
                mine[v[i]]--;
            }
        }
    }
    else{
        for(int i=0; i<v.size(); i++){
            if(hint[v[i]] == true){
                mine[v[i]]++;
            }
        }
    }
}

//原始的backtracking
int minesweeper(vector<int> &mine,  vector<bool> hint, int place, bool bomb){
    if(hint[place] == true){
        place++;
        return place;
    }
    else{
        mine[place]=bomb;
        change_hint(mine, hint, place, bomb);
        if(check_hint_greater_than_zero(mine, hint) == true){
            place++;
            return place;
        }
        else{
            return place;
        }
    }
}

//加了 mrv 的 backtracking
int minesweeper_mrv(vector<int> &mine, vector<bool> hint, int place, bool bomb, vector<int> &domain){
    if(hint[place] == true){
        place++;
        return place;
    }
    else if(domain[place] == 1 || domain[place] == 0){
        mine[place] = bomb;
        if(bomb == 1){
            change_hint(mine, hint, place, bomb);
        }
        change_domain(mine, hint, domain);
        domain[place] = -1;
        int ans = find_domain_1(domain);
        return ans;
    }
    else{
        mine[place]=bomb;
        change_hint(mine, hint, place, bomb);
        change_domain(mine, hint, domain);
        domain[place] = -1;
        if(check_hint_greater_than_zero(mine, hint) == true){
            int ans = find_domain_1(domain);
            return ans;
        }
        else{
            return place;
        }
    }
}

//加了 mrv & degree 的 backtracking
int minesweeper_degree(vector<int> &mine, vector<bool> hint, int place, bool bomb, vector<int> &domain, vector<int> degree, vector<bool> &sign){
    if(hint[place] == true){
        place++;
        return place;
    }
    else if(domain[place] == 1 || domain[place] == 0){
        mine[place] = bomb;
        if(bomb == 1){
            change_hint(mine, hint, place, bomb);
        }
        change_domain(mine, hint, domain);
        domain[place] = -1;
        sign[place] = true;
        int ans = find_degree(domain, hint, degree, sign);
        return ans;
    }
    else{
        mine[place]=bomb;
        change_hint(mine, hint, place, bomb);
        change_domain(mine, hint, domain);
        domain[place] = -1;
        sign[place] = true;
        if(check_hint_greater_than_zero(mine, hint) == true){
            int ans = find_degree(domain, hint, degree, sign);
            return ans;
        }
        else{
            return place;
        }
    }
}

int main(int argc, const char * argv[]) {
    //計算時間
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    int boardsizex;
    int boardsizey;
    int mines;
    cin>>boardsizex>>boardsizey>>mines;
    vector<int> mine;
    for(int i=0; i<boardsizex; i++){
        for(int j=0; j<boardsizey; j++){
            int t;
            cin>>t;
            mine.push_back(t);
        }
    }
    vector<bool> hint;
    for(int i=0; i<mine.size(); i++){
        if(mine[i]!=-1){
            hint.push_back(true);
        }
        else{
            hint.push_back(false);
        }
    }
    
    vector<int> h_mrv;
    for(int i=0; i<hint.size(); i++){
        if(hint[i] == true){
            h_mrv.push_back(3);
        }
        else{
            h_mrv.push_back(2);
        }
    }
    
    vector<int> h_degree;
    for(int i=0; i<hint.size(); i++){
        h_degree.push_back(-1);
    }
    int t = sqrt(hint.size());
    int arr[t][t];
    for(int i=0; i<t; i++){
        for(int j=0; j<t; j++){
            arr[i][j] = i*t+j;
        }
    }
    for(int i=0; i<hint.size(); i++){
        if(hint[i] == true){
            int x=(i/t);
            int y=(i%t);
            if(y-1 >= 0 && hint[arr[x][y-1]] == false){
                h_degree[arr[x][y-1]] += 1;
            }
            if(y+1 < t && hint[arr[x][y+1]] == false){
                h_degree[arr[x][y+1]] += 1;
            }
            if(x-1 >= 0 && hint[arr[x-1][y]] == false){
                h_degree[arr[x-1][y]] += 1;
            }
            if(x-1 >= 0 && y-1 >= 0 && hint[arr[x-1][y-1]] == false){
                h_degree[arr[x-1][y-1]] += 1;
            }
            if(x-1 >= 0 && y+1 < t && hint[arr[x-1][y+1]] == false){
                h_degree[arr[x-1][y+1]] += 1;
            }
            if(x+1 < t && hint[arr[x+1][y]] == false){
                h_degree[arr[x+1][y]] += 1;
            }
            if(x+1 < t && y-1 >= 0 && hint[arr[x+1][y-1]] == false){
                h_degree[arr[x+1][y-1]] += 1;
            }
            if(x+1 < t && y+1 < t && hint[arr[x+1][y+1]] == false){
                h_degree[arr[x+1][y+1]] += 1;
            }
        }
    }
    
    vector<bool> sign;
    for(int i=0; i<mine.size(); i++){
        if(mine[i] != -1){
            sign.push_back(true);
        }
        else{
            sign.push_back(false);
        }
    }
    
    vector<int> h_lcv;
    for(int i=0; i<h_mrv.size(); i++){
        h_lcv.push_back(h_mrv[i]);
    }
    
    int place = 0;
    bool bomb = 0;
    pair<int, bool> p;
    p.first = place;
    p.second = 0;
    s.push(p);
    p.first = place;
    p.second = 1;
    s.push(p);
    bool flag = 0;
    int count = 0;
    
    if(MRV_true == false && MRV_degree_true == false && MRV_degree_LCV_true == false){
        while(place < mine.size()){
            count++;
            if(flag == 0){
                place = s.top().first;
                bomb = s.top().second;
                s.pop();
            }
            else{
                flag = 0;
            }
            int temp;
            temp = minesweeper(mine, hint, place, bomb);
            if(temp != place){
                place = temp;
            }
            else{
                place = s.top().first;
                bomb = s.top().second;
                s.pop();
                temp = minesweeper(mine, hint, place, bomb);
                place = temp;
            }
            
            if(place == mine.size() && check_hint_zero(mine, hint) == true){
                break;
            }
            else if(place != mine.size() && hint[place] == true){
                p.first = place;
                p.second = 0;
                s.push(p);
            }
            else if(place != mine.size()){
                p.first = place;
                p.second = 0;
                s.push(p);
                p.first = place;
                p.second = 1;
                s.push(p);
            }
            else{
                place = s.top().first;
                bomb = s.top().second;
                s.pop();
                flag = 1;
            }
        }
    }
    else if(MRV_true == true){
        stack<int> walkthrough;
        while(place < mine.size()){
            count++;
            if(flag == 0){
                place = s.top().first;
                bomb = s.top().second;
                s.pop();
            }
            else{
                while(walkthrough.top() != place){
                    if(mine[walkthrough.top()] == 1){
                        change_hint(mine, hint, walkthrough.top(), 0);
                    }
                    mine[walkthrough.top()] = -1;
                    h_mrv[walkthrough.top()] = 2;
                    walkthrough.pop();
                }
                if(mine[walkthrough.top()] == 1){
                    change_hint(mine, hint, walkthrough.top(), 0);
                }
                mine[walkthrough.top()] = -1;
                h_mrv[walkthrough.top()] = 1;
                walkthrough.pop();
                flag = 0;
            }
            walkthrough.push(place);
            int temp;
            temp = minesweeper_mrv(mine, hint, place, bomb, h_mrv);
            if(temp != place){
                place = temp;
            }
            else{
                place = s.top().first;
                bomb = s.top().second;
                s.pop();
                temp = minesweeper_mrv(mine, hint, place, bomb, h_mrv);
                place = temp;
            }
            
            if(place == mine.size() && check_hint_zero(mine, hint) == true){
                break;
            }
            else if(place != mine.size() && hint[place] == true){
                p.first = place;
                p.second = 0;
                s.push(p);
            }
            else if(place != mine.size() && h_mrv[place] == 1){
                p.first = place;
                p.second = 1;
                s.push(p);
            }
            else if(place != mine.size() && h_mrv[place] == 0){
                p.first = place;
                p.second = 0;
                s.push(p);
            }
            else if(place != mine.size()){
                p.first = place;
                p.second = 0;
                s.push(p);
                p.first = place;
                p.second = 1;
                s.push(p);
            }
            else{
                place = s.top().first;
                bomb = s.top().second;
                s.pop();
                flag = 1;
            }
        }
    }
    else if(MRV_degree_true == true){
        stack<int> walkthrough;
        while(place < mine.size()){
            count++;
            if(flag == 0){
                place = s.top().first;
                bomb = s.top().second;
                s.pop();
            }
            else{
                while(walkthrough.top() != place){
                    if(mine[walkthrough.top()] == 1){
                        change_hint(mine, hint, walkthrough.top(), 0);
                    }
                    mine[walkthrough.top()] = -1;
                    h_mrv[walkthrough.top()] = 2;
                    sign[walkthrough.top()] = false;
                    walkthrough.pop();
                }
                if(mine[walkthrough.top()] == 1){
                    change_hint(mine, hint, walkthrough.top(), 0);
                }
                mine[walkthrough.top()] = -1;
                h_mrv[walkthrough.top()] = 1;
                sign[walkthrough.top()] = false;
                walkthrough.pop();
                flag = 0;
            }
            walkthrough.push(place);
            int temp;
            temp = minesweeper_degree(mine, hint, place, bomb, h_mrv, h_degree, sign);
            if(temp != place){
                place = temp;
            }
            else{
                place = s.top().first;
                bomb = s.top().second;
                s.pop();
                cout<<place<<" "<<bomb<<endl;
                place = temp;
            }
            
            if(place == mine.size() && check_hint_zero(mine, hint) == true && mine_equal(mine, mines)){
                break;
            }
            else if(place != mine.size() && hint[place] == true){
                p.first = place;
                p.second = 0;
                s.push(p);
            }
            else if(place != mine.size() && h_mrv[place] == 1){
                p.first = place;
                p.second = 1;
                s.push(p);
            }
            else if(place != mine.size() && h_mrv[place] == 0){
                p.first = place;
                p.second = 0;
                s.push(p);
            }
            else if(place != mine.size()){
                p.first = place;
                p.second = 0;
                s.push(p);
                p.first = place;
                p.second = 1;
                s.push(p);
            }
            else{
                place = s.top().first;
                bomb = s.top().second;
                s.pop();
                flag = 1;
            }
        }
        
    }
    else{
        stack<int> walkthrough;
        while(place < mine.size()){
            count++;
            if(flag == 0){
                place = s.top().first;
                bomb = s.top().second;
                s.pop();
            }
            else{
                while(walkthrough.top() != place){
                    if(mine[walkthrough.top()] == 1){
                        change_hint(mine, hint, walkthrough.top(), 0);
                    }
                    mine[walkthrough.top()] = -1;
                    h_mrv[walkthrough.top()] = 2;
                    sign[walkthrough.top()] = false;
                    walkthrough.pop();
                }
                if(mine[walkthrough.top()] == 1){
                    change_hint(mine, hint, walkthrough.top(), 0);
                }
                mine[walkthrough.top()] = -1;
                h_mrv[walkthrough.top()] = 1;
                sign[walkthrough.top()] = false;
                walkthrough.pop();
                flag = 0;
            }
            walkthrough.push(place);
            int temp;
            temp = minesweeper_degree(mine, hint, place, bomb, h_mrv, h_degree, sign);
            if(temp != place){
                place = temp;
            }
            else{
                place = s.top().first;
                bomb = s.top().second;
                s.pop();
                cout<<place<<" "<<bomb<<endl;
                place = temp;
            }
            
            if(place == mine.size() && check_hint_zero(mine, hint) == true && mine_equal(mine, mines)){
                break;
            }
            else if(place != mine.size() && hint[place] == true){
                p.first = place;
                p.second = 0;
                s.push(p);
            }
            else if(place != mine.size() && h_mrv[place] == 1){
                p.first = place;
                p.second = 1;
                s.push(p);
            }
            else if(place != mine.size() && h_mrv[place] == 0){
                p.first = place;
                p.second = 0;
                s.push(p);
            }
            else if(place != mine.size()){
                int a0 = find_constraint(mine, hint, h_mrv, place, bomb);
                int a1 = find_constraint(mine, hint, h_mrv, place, bomb);
                if(a0>a1){
                    p.first = place;
                    p.second = 1;
                    s.push(p);
                    p.first = place;
                    p.second = 0;
                    s.push(p);
                }
                else{
                    p.first = place;
                    p.second = 0;
                    s.push(p);
                    p.first = place;
                    p.second = 1;
                    s.push(p);
                }
            }
            else{
                place = s.top().first;
                bomb = s.top().second;
                s.pop();
                flag = 1;
            }
        }
    }
    
    cout<<endl;
    output_board(mine);
    
    end = clock();
    cpu_time_used = (double)(end-start)/CLOCKS_PER_SEC;
    
    cout<<endl<<"cpu time: "<<cpu_time_used<<endl;
    cout<<"expand node: "<<count<<endl;
    
    return 0;
}
