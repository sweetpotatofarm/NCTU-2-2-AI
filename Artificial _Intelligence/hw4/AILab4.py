#!/usr/bin/env python
# coding: utf-8

# In[938]:


import numpy as np
# 4, 10, 34
attributes = 10
# 3, 7, 2
classes = 7
# 150, 214, 351
Instances = 214
trainingDataNum = int(Instances*(2/3))
validationDataNum = Instances - trainingDataNum
treeNum = 100


# In[939]:


# read file
# iris.data, glass.data, ionosphere.data
fp = open('glass.data', "r")
lines = fp.readlines()
data = []
# may have to change between len(lines) or len(lines)-1 through different data
for i in range(len(lines)):
    lines[i] = lines[i].split(',')
    lines[i][attributes] = lines[i][attributes][:-1]
    data.append(lines[i])
fp.close()

for i in range(len(data)):
    for j in range(attributes):
        data[i][j] = float(data[i][j])
        
# training subset and validation subset
import random

random.shuffle(data)
trainingSubset = []
validationSubset = []
for i in range(trainingDataNum):
    trainingSubset.append(data[i])
for i in range(trainingDataNum, trainingDataNum+validationDataNum):
    validationSubset.append(data[i])


# In[940]:


import random
import math

# attribute bagging
def selectAttributes(n):
    temp = []
    for i in range(n-1):
        temp.append(i)
    random.shuffle(temp)
    num = int(math.sqrt(n))
    ans = []
    for i in range(num):
        ans.append(temp[i])
    return ans
# tree bagging
def selectData(data, n):
    ans = []
    for i in range(n):
        tt = random.randint(0, len(data)-1)
        ans.append(data[tt])
    return ans


# In[941]:


def splittingNode(trainingData, trainingAttribute):
    # select a attribute
    bestAttribute = 0
    bestThreshold = 0
    lowestImpurity = 1
    for i in range(len(trainingAttribute)):
        temparr = []
        V = []
        # select a threshold for a given attribute
        for j in range(len(trainingData)):
            T = []
            T.append(trainingData[j][trainingAttribute[i]])
            if(trainingData[j][trainingAttribute[i]] not in V):
                V.append(trainingData[j][trainingAttribute[i]])
            T.append(trainingData[j][len(trainingData[0])-1])
            temparr.append(T)
        # temparr saves one of the attribute data and a tag
        temparr.sort(key = lambda s: s[0])
        # V saves all values of given attributes 
        V.sort()
        threshold = []
        # threshold are decided by V
        for j in range(len(V)-1):
            threshold.append((V[j]+V[j+1])/2)

        # decide which threshold is better
        betterThreshold = 0
        lowestGini = 1
        for j in range(len(threshold)):
            alpha = []
            beta = []
            # use threshold to devide data into two sets, which is alpha and beta
            for k in range(len(temparr)):
                if(temparr[k][0] > threshold[j]):
                    alpha.append(temparr[k][1])
                else:
                    beta.append(temparr[k][1])
            alpha.sort()
            beta.sort()
            # alphaCount and betaCount holds the sum of every class
            alphaCount = []
            betaCount = []
            string = alpha[0]
            count = 0
            for k in range(len(alpha)):
                if(string == alpha[k]):
                    count+=1
                else:
                    alphaCount.append(count)
                    string = alpha[k]
                    count = 1
            alphaCount.append(count)
            string = beta[0]
            count = 0
            for k in range(len(beta)):
                if(string == beta[k]):
                    count+=1
                else:
                    betaCount.append(count)
                    string = beta[k]
                    count = 1
            betaCount.append(count)
            # calculate Gini index
            alphaGini = 1
            betaGini = 1
            alphaSum = len(alpha)
            betaSum = len(beta)
            for k in range(len(alphaCount)):
                alphaGini-=(alphaCount[k]/alphaSum)*(alphaCount[k]/alphaSum)
            for k in range(len(betaCount)):
                betaGini-=(betaCount[k]/betaSum)*(betaCount[k]/betaSum)
            Gini = (alphaSum/(alphaSum+betaSum))*alphaGini+(betaSum/(alphaSum+betaSum))*betaGini
            if(Gini < lowestGini):
                lowestGini = Gini
                betterThreshold = threshold[j]

        if(lowestGini < lowestImpurity):
            lowestImpurity = lowestGini
            bestAttribute = trainingAttribute[i]
            bestThreshold = betterThreshold

    newAlpha = []
    newBeta = []
    for i in range(len(trainingData)):
        if(trainingData[i][bestAttribute] < bestThreshold):
            newAlpha.append(trainingData[i])
        else:
            newBeta.append(trainingData[i])

    ans = []
    ans.append(lowestImpurity)
    ans.append(bestAttribute)
    ans.append(bestThreshold)
    ans.append(newAlpha)
    ans.append(newBeta)
    return ans

# following is used to implement extremely random forest
# import random
# def splittingNode(trainingData, trainingAttribute):
#     i = random.randint(0,len(trainingAttribute)-1)
#     temparr = []
#     V = []
#     # select a threshold for a given attribute
#     for j in range(len(trainingData)):
#         T = []
#         T.append(trainingData[j][trainingAttribute[i]])
#         if(trainingData[j][trainingAttribute[i]] not in V):
#             V.append(trainingData[j][trainingAttribute[i]])
#         T.append(trainingData[j][len(trainingData[0])-1])
#         temparr.append(T)
#     # temparr saves one of the attribute data and a tag
#     temparr.sort(key = lambda s: s[0])
#     # V saves all values of given attributes 
#     V.sort()
#     threshold = []
#     # threshold are decided by V
#     for j in range(len(V)-1):
#         threshold.append((V[j]+V[j+1])/2)

#     # decide which threshold is better
#     betterThreshold = 0
#     lowestGini = 1
#     for j in range(len(threshold)):
#         alpha = []
#         beta = []
#         # use threshold to devide data into two sets, which is alpha and beta
#         for k in range(len(temparr)):
#             if(temparr[k][0] > threshold[j]):
#                 alpha.append(temparr[k][1])
#             else:
#                 beta.append(temparr[k][1])
#         alpha.sort()
#         beta.sort()
#         # alphaCount and betaCount holds the sum of every class
#         alphaCount = []
#         betaCount = []
#         string = alpha[0]
#         count = 0
#         for k in range(len(alpha)):
#             if(string == alpha[k]):
#                 count+=1
#             else:
#                 alphaCount.append(count)
#                 string = alpha[k]
#                 count = 1
#         alphaCount.append(count)
#         string = beta[0]
#         count = 0
#         for k in range(len(beta)):
#             if(string == beta[k]):
#                 count+=1
#             else:
#                 betaCount.append(count)
#                 string = beta[k]
#                 count = 1
#         betaCount.append(count)
#         # calculate Gini index
#         alphaGini = 1
#         betaGini = 1
#         alphaSum = len(alpha)
#         betaSum = len(beta)
#         for k in range(len(alphaCount)):
#             alphaGini-=(alphaCount[k]/alphaSum)*(alphaCount[k]/alphaSum)
#         for k in range(len(betaCount)):
#             betaGini-=(betaCount[k]/betaSum)*(betaCount[k]/betaSum)
#         Gini = (alphaSum/(alphaSum+betaSum))*alphaGini+(betaSum/(alphaSum+betaSum))*betaGini
#         if(Gini < lowestGini):
#             lowestGini = Gini
#             betterThreshold = threshold[j]
#     newAlpha = []
#     newBeta = []
#     bestAttribute = i
#     bestThreshold = betterThreshold
#     for i in range(len(trainingData)):
#         if(trainingData[i][bestAttribute] < bestThreshold):
#             newAlpha.append(trainingData[i])
#         else:
#             newBeta.append(trainingData[i])
#     ans = []
#     ans.append(lowestGini)
#     ans.append(bestAttribute)
#     ans.append(bestThreshold)
#     ans.append(newAlpha)
#     ans.append(newBeta)
#     return ans


# In[942]:


class Tree():
    def __init__(self):
        self.root = Node(0, 0)
class Node():
    def __init__(self, attribute, threshold):
        self.attribute = attribute
        self.threshold = threshold
        self.leafnode = False
        self.classification = ""


# In[943]:


# build a tree
def buildTree(root, trainingData, trainingAttributes):
    finish = True
    if(trainingData != []):
        string = trainingData[0][len(trainingData[0])-1]
        for i in range(len(trainingData)):
            if(trainingData[i][len(trainingData[0])-1] != string):
                finish = False
                break
        if(finish == False):
            returnData = splittingNode(trainingData, trainingAttributes)
            Node1 = Node(0,0)
            Node2 = Node(0,0)
            root.attribute = returnData[1]
            root.threshold = returnData[2]
            root.left = Node1
            root.right = Node2
            trainingAttributes = selectAttributes(attributes)
            buildTree(Node1, returnData[3], trainingAttributes)
            buildTree(Node2, returnData[4], trainingAttributes)
        else:
            root.leafnode = True
            root.classification = trainingData[0][len(trainingData[0])-1]
    else:
        root.leafnode = True
        root.classification = ""


# In[944]:


# main
sum = 0
# build a forest
for n in range(treeNum):
    decisionTree = Tree()
    trainingData = selectData(trainingSubset, trainingDataNum)
    buildTree(decisionTree.root, trainingData, selectAttributes(attributes))
    validationData = selectData(validationSubset, validationDataNum)

    correct = 0
    wrong = 0
    for i in range(len(validationData)):
        position = decisionTree.root
        while(position.leafnode == False):
            if(validationData[i][position.attribute] > position.threshold):
                position = position.right
            else:
                position = position.left
        if(validationData[i][len(validationData[i])-1] == position.classification):
            correct+=1
        else:
            wrong+=1
    sum+=(correct/(correct+wrong))

print("glass.data")
print(sum/treeNum)


# In[ ]:




