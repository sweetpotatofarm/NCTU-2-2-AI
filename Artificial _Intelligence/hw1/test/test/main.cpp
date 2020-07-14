//
//  main.cpp
//  test
//
//  Created by 蕭楚澔 on 2020/4/6.
//  Copyright © 2020 Bob. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

void f(vector<int> &v){
    v.pop_back();
    return;
}

int main(int argc, const char * argv[]) {
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    f(v);
    for (int i=0; i<v.size(); i++) {
        cout<<v[i]<<endl;
    }
    
    return 0;
}
