#!/usr/bin/env python
# coding: utf-8

# In[35]:


import numpy as np
import math
#boardsizex = 16
#boardsizey = 16
#mines = 25
#safe_cells = round(math.sqrt(boardsizex*boardsizey))


# In[36]:


from random import sample
position_list = [i for i in range(0,boardsizex*boardsizey)]
mines_position = sample(position_list, mines)
arr = []
for i in range(0, boardsizex):
    col = []
    for j in range(0, boardsizey):
        col.append(0)
    arr.append(col)
for i in range(0, len(mines_position)):
    bomb = mines_position[i]
    m = (int)(bomb/boardsizey)
    n = bomb%boardsizey
    arr[m][n] = 'x'
for i in range(len(arr)):
    for j in range(len(arr[i])):
        if(arr[i][j] == 'x'):
            if(i-1 >= 0 and j-1 >= 0 and arr[i-1][j-1] != 'x'):
                arr[i-1][j-1]+=1
            if(i-1 >= 0 and arr[i-1][j] != 'x'):
                arr[i-1][j]+=1
            if(i-1 >= 0 and j+1 < boardsizey and arr[i-1][j+1] != 'x'):
                arr[i-1][j+1]+=1
            if(j-1 >= 0 and arr[i][j-1] != 'x'):
                arr[i][j-1]+=1
            if(j+1 < boardsizey and arr[i][j+1] != 'x'):
                arr[i][j+1]+=1
            if(i+1 < boardsizex and j-1 >= 0 and arr[i+1][j-1] != 'x'):
                arr[i+1][j-1]+=1
            if(i+1 < boardsizex and arr[i+1][j] != 'x'):
                arr[i+1][j]+=1
            if(i+1 < boardsizex and j+1 < boardsizey and arr[i+1][j+1] != 'x'):
                arr[i+1][j+1]+=1


# In[37]:


def printarray(array):
    for i in range(len(array)):
        for j in range(len(array[i])):
            print(array[i][j], end = ' ')
        print('\n')


# In[38]:


# printarray(arr)


# In[39]:


player_arr = []
for i in range(0, boardsizex):
    col = []
    for j in range(0, boardsizey):
        col.append(-1)
    player_arr.append(col)


# In[40]:


from itertools import combinations

def minesweeperFinish(array):
    flag = True
    for i in range(len(array)):
        for j in range(len(array[i])):
            if(array[i][j] == -1):
                flag = False
                break
        if(flag == False):
            break
    return flag

def negative(num):
    ans = num - 2*num
    return ans

def combi(arr, r):
    C = list(combinations(arr, r))
    for i in range(len(C)):
        C[i] = list(C[i])
    return C


# In[41]:


KB = []
safepoint_list = [i for i in range(0,boardsizex*boardsizey)]
temp_position = sample(position_list, safe_cells+mines)
safepoint_position = []
for i in range(len(temp_position)):
    position = temp_position[i]
    m = (int)(position/boardsizey)
    n = position%boardsizey
    if(arr[m][n] != 'x' and len(safepoint_position) < safe_cells):
        safepoint_position.append(position)
        player_arr[m][n] = 0
KB0 = []


# In[42]:


pre_list = []
for i in range(len(safepoint_position)):
    temp_list = []
    position = safepoint_position[i]
    temp_list.append(negative(position)-1)
    KB.append(temp_list)

#print(KB)
#print(KB0)


# In[43]:


def deleteLoose(twoDArr, li):
    delete_arr = []
    for i in range(len(twoDArr)):
        deleteItem = []
        arr = li.copy()
        for j in range(len(twoDArr[i])):
            for k in range(len(li)):
                if(twoDArr[i][j] == li[k] and (li[k] in deleteItem) == False):
                    deleteItem.append(li[k])
        for t in range(len(deleteItem)):
            arr.remove(deleteItem[t])
        if(arr == []):
            delete_arr.append(i)
    for j in range(len(delete_arr)-1, -1, -1):
        del twoDArr[delete_arr[j]]

def isStricterThan(twoDArr, li):
    deleteItem = []
    for i in range(len(twoDArr)):
        arr = twoDArr[i].copy()
        for j in range(len(twoDArr[i])):
            for k in range(len(li)):
                if(twoDArr[i][j] == li[k] and (li[k] in arr) == True):
                    arr.remove(li[k])
        if(arr == []):
            return False
    return True

def countComplement(arrA, arrB):
    count = 0
    for i in range(len(arrA)):
        for j in range(len(arrB)):
            if(arrA[i]+arrB[j] == 0):
                count+=1
    return count

def GenerateNewClause(arrA, arrB):
    newArrA = arrA.copy()
    newArrB = arrB.copy()
    for i in range(len(arrA)):
        for j in range(len(arrB)):
            if(arrA[i]+arrB[j] == 0):
                del newArrA[i]
                del newArrB[j]
                
    nnewArrA = newArrA.copy()
    nnewArrB = newArrB.copy()
    for i in range(len(newArrA)):
        for j in range(len(newArrB)):
            if(newArrA[i] == newArrB[j]):
                nnewArrB.remove(newArrB[j])
    returnList = []
    returnList = nnewArrA + nnewArrB
    return returnList       

def insertKB(KB, KB0, insertList):
    for i in range(len(KB0)):
        if(countComplement(KB0[i], insertList) == 1):
            insertList = GenerateNewClause(KB0[i], insertList)
    if((insertList in KB0) == False):
        if((insertList in KB) == False and isStricterThan(KB, insertList) == True):
            deleteLoose(KB, insertList)
#             print(insertList)
            KB.append(insertList)

def GenerateClauseFromHint(KB0, KB, arr, player_arr, x, y, position, boardsizex, boardsizey):
        N = arr[x][y]
        M = 0
        t_list = []
        if(x-1 >= 0 and y-1 >= 0 and player_arr[x-1][y-1] == -1):
            M+=1
            t_list.append(position-boardsizey-1+1)
        if(x-1 >= 0 and player_arr[x-1][y] == -1):
            M+=1
            t_list.append(position-boardsizey+1)
        if(x-1 >= 0 and y+1 < boardsizey and player_arr[x-1][y+1] == -1):
            M+=1
            t_list.append(position-boardsizey+1+1)
        if(y-1 >= 0 and player_arr[x][y-1] == -1):
            M+=1
            t_list.append(position-1+1)
        if(y+1 < boardsizey and player_arr[x][y+1] == -1):
            M+=1
            t_list.append(position+1+1)
        if(x+1 < boardsizex and y-1 >= 0 and player_arr[x+1][y-1] == -1):
            M+=1
            t_list.append(position+boardsizey-1+1)
        if(x+1 < boardsizex and player_arr[x+1][y] == -1):
            M+=1
            t_list.append(position+boardsizey+1)
        if(x+1 < boardsizex and y+1 < boardsizey and player_arr[x+1][y+1] == -1):
            M+=1
            t_list.append(position+boardsizey+1+1)
        if(x-1 >= 0 and y-1 >= 0 and player_arr[x-1][y-1] == 1):
            N-=1
        if(x-1 >= 0 and player_arr[x-1][y] == 1):
            N-=1
        if(x-1 >= 0 and y+1 < boardsizey and player_arr[x-1][y+1] == 1):
            N-=1
        if(y-1 >= 0 and player_arr[x][y-1] == 1):
            N-=1
        if(y+1 < boardsizey and player_arr[x][y+1] == 1):
            N-=1
        if(x+1 < boardsizex and y-1 >= 0 and player_arr[x+1][y-1] == 1):
            N-=1
        if(x+1 < boardsizex and player_arr[x+1][y] == 1):
            N-=1
        if(x+1 < boardsizex and y+1 < boardsizey and player_arr[x+1][y+1] == 1):
            N-=1
        if(N == M):
            for j in range(len(t_list)):
                temp_list = []
                temp_list.append(t_list[j])
                insertKB(KB, KB0, temp_list)
        elif(N == 0):
            for j in range(len(t_list)):
                temp_list = []
                temp_list.append(negative(t_list[j]))
                insertKB(KB, KB0, temp_list)
        else:
            comb = combi(t_list, M-N+1)
            for j in range(len(comb)):
                temp_list = []
                for k in range(len(comb[j])):
                    temp_list.append(comb[j][k])
                insertKB(KB, KB0, temp_list)
            temp_list = []
            comb = combi(t_list, N+1)
            for j in range(len(comb)):
                temp_list = []
                for k in range(len(comb[j])):
                    temp_list.append(negative(comb[j][k]))
                insertKB(KB, KB0, temp_list)
                


# In[44]:


noInsert = False
while((len(KB) != 0 and noInsert == False)):
    i = 0
    while(len(KB[i]) != 1):
        if(i == len(KB)-1):
            break
        i+=1
    if(len(KB[i]) == 1):
        singleClause = KB[i]
        cell = KB[i][0]
        del KB[i]
        position = 0
        if(cell>0):
            position = cell - 1
            x = (int)(position/boardsizey)
            y = position%boardsizey
            player_arr[x][y] = 1
            KB0.append(singleClause)
            j = 0
            while(j < len(KB)):
                if(countComplement(singleClause, KB[j]) == 1):
                    newClause = GenerateNewClause(singleClause, KB[j])
                    insertKB(KB, KB0, newClause)
                j+=1
        else:
            position = cell - 2*cell -1
            x = (int)(position/boardsizey)
            y = position%boardsizey
            player_arr[x][y] = 0
            KB0.append(singleClause)
            j = 0
            while(j < len(KB)):
                if(countComplement(singleClause, KB[j]) == 1):
                    newClause = GenerateNewClause(singleClause, KB[j])
                    insertKB(KB, KB0, newClause)
                j+=1
            GenerateClauseFromHint(KB0, KB, arr, player_arr, x, y, position, boardsizex, boardsizey)
    else:
        noInsert = True
        insertList = []
        for j in range(0, len(KB)):
            for k in range(j+1, len(KB)):
                if(countComplement(KB[k], KB[j]) == 1):
                    newClause = GenerateNewClause(KB[k], KB[j])
                    if((newClause in KB) == False and isStricterThan(KB, newClause) == True):
                        insertList.append(newClause)
        
        if(len(insertList) != 0):
            noInsert = False
            for t in range(len(insertList)):
                insertKB(KB, KB0, insertList[t])


# In[45]:


#print("KB: ",end='')
#print(KB)
#print("KB0: ",end='')
#print(KB0)
#print(len(KB0))
#printarray(player_arr)
#printarray(arr)

#flag = True
if(minesweeperFinish(player_arr) == True):
#    print("game finished!")
    isFinish = True
else:
#    print("game stucked!")
    isFinish = False


# In[ ]:




