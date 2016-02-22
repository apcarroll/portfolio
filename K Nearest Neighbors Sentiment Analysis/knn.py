import csv
import math
from nltk.probability import *
from nltk.cluster import KMeansClusterer, GAAClusterer, cosine_distance
from nltk.stem import *
import json
from nltk import stem
from nltk.cluster import *
import nltk.cluster.util
from time import *
json_file = 'groupon_data.json'

truePositive=0
trueNegative=0
falsePositive=0
falseNegative=0

def readFile(file_name):
    input_file = open(file_name,'r');
    data_list = []
    for line in input_file:
        mydict = json.loads(line)
        mylist = []
        mylist.append(mydict['label'])
        mylist.append(mydict['list'])
        data_list.append(mylist)
    input_file.close()
    return data_list



    
    

'''
def knn(word,k,data_list):
    stemmer = stem.snowball.EnglishStemmer()
    term = stemmer.stem(word)l

    print term
    #nltk.cluster.util.cosine_distanc
le(term,x)
    #clusterer = cluster.KMeansClusterer(2, cosine_distance, initial_means=means)
    for x in data_list[10]:
        #print x
        distance = nltk.cluster.util.cosine_distance(term,x)
        print distance
       
'''
def printResults(results,resultsB,k,data_list,x,totalNegative):
        totalNegative=totalNegative*2
        numNegative=0
        numPositive=0
        predictPositive=False
        positive=False
        results.sort()
        topK=[]
        i=0
        while i<k:
            topK.append(0)
            i+=1
        i=len(results)-1
        #print "the length of results -1 is "
        #print i
        j=0
        properI=i
        while j < k:
            topK[j]=results[i]
            i=i-1
            j=j+1
        
                
        
        thresh=topK[k-1]
       
        topKIndicies=[]
        p=0
        while p < len(resultsB):
            if resultsB[p] >= thresh:
                topKIndicies.append(p+1)
            p=p+1
        for Index in topKIndicies:
            """
            print "The index is "
            print Index
            print "The value is"
            print data_list[Index][0]
            """
            if data_list[Index][0]=='0':
                numNegative+=1
                
            if data_list[Index][0]=='1':
                numPositive+=1
        """
        print "numPositive"
        print numPositive
        print "numNegative"
        print numNegative
        """
        #x=number of negative reviews in training/ all reviews in training
        
        threshHold=0
        if numPositive==0:
            threshHold=.65
        else:
            #threshHold=float(numNegative)/numPositive
            threshHold=float(numNegative)/k
        if threshHold>=totalNegative:
            predictPositive=False
        else:
            predictPositive=True
        print "Threshhold is"
        print threshHold
        print "total negative is"
        print totalNegative
        


        """
        if numNegative >= numPositive:
            predictPositive=False
            #print "predicted negative"
         
        else:
            predictPositive=True
            #print "predicted positive"
        """
        if data_list[x][0]=='0':
            positive=False
            #print "is negative"
        if data_list[x][0]=='1':
            positive=True
            #print "is positive"
        """
        print "result"
        print positive
        """
        if positive==True and predictPositive==True:
            global truePositive
            #print "true positive detected"
            truePositive+=1
        if positive==False and predictPositive==False:
            global trueNegative
            #print "true negative detected"
            trueNegative+=1
        if positive==True and predictPositive==False:
            global falseNegative
            #print "false negative detected"
            falseNegative+=1
        if positive==False and predictPositive==True:
            global falsePositive 
            #print "false positive detected"
            falsePositive+=1




def knn(trainingStart,trainingEnd,testStart,testEnd,data_list,k):
    trainingVector=[]
    trainingWords=[]
    trainingNegative=0    


    x=trainingStart
    count=0
    while x< trainingEnd:
        if(data_list[x][0]=='0'):
            trainingNegative+=1
        for word in data_list[x][1]:
            if word not in trainingWords:
                trainingWords.append(word)
                count+=1
           
        x+=1
    z=0
  

    

    trainingSet=[]
    x=trainingStart
    y=0
    
    x=0
    while x < trainingEnd-trainingStart:
        trainingSet.append([])
        y=0
        while y < count:
            trainingSet[x].append(0)
            y=y+1
        x=x+1
    #print "The size of the training set is "
    #print x
    limit=x
    totalNegative=float(trainingNegative)/limit
    y=trainingStart
    while y<trainingEnd:
        for word in data_list[y][1]:
            if word  in trainingWords:
                index=trainingWords.index(word)
                trainingSet[y-trainingStart][index]+=1
        y+=1



    x=testStart
    while x<testEnd:
        testVector=[]
        z=0
        while z<count:
            testVector.append(0)
            z+=1        
        for word in data_list[x][1]:
            if word in trainingWords:
                index=trainingWords.index(word)
                testVector[index]+=1

        y=trainingStart
        
        #while y < trainingEnd:
        results=[]
        resultsB=[]
        while y< limit:
            
            #do the math
            testLen=0
            trainLen=0
            dotProduct=0
            for te in testVector:
                testLen+=math.pow(te,2)
            #print "the y in the training set is "
            #print y
            for tr in trainingSet[y]:
                trainLen+=math.pow(tr,2)
            #testLen=math.sqrt(testLen)
            #trainLen=math.sqrt(trainLen)
            i=0
            for C in testVector:
                tmp=C*trainingSet[y][i]
                i+=1
                dotProduct+=tmp
            answer=dotProduct/(testLen*trainLen)
            #print "The answer that I got is "
            #print answer
             
            results.append(answer)
            resultsB.append(answer)
            if(y==limit-1):
                printResults(results,resultsB,k,data_list,x,totalNegative)
                del results[:]
                del resultsB[:]
            y+=1 

        
        print "true positive"
        print truePositive
        print "false positive"
        print falsePositive
        print "true negative"
        print trueNegative
        print "false negative"
        print falseNegative
        
        if(x==testEnd-1):
            global truePositive
            global trueNegative
            global falsePositive
            global falseNegative
            precision=float(truePositive)/(truePositive+falsePositive)
            recall=float(truePositive)/(truePositive+falseNegative)
            trueNegativeRate=float(trueNegative)/(trueNegative+falsePositive)  
            accuracy=float(truePositive+trueNegative)/(truePositive+trueNegative+falsePositive+falseNegative) 
            fOne = 2*((precision*recall)/(precision+recall))        
        
            print "precision"
            print precision
            print "recall"
            print recall
            print "trueNegativeRate"
            print trueNegativeRate
            print "accuracy"
            print accuracy
            print "f1 value"
            print fOne


        x+=1
        
    
    


if __name__ == "__main__":
    data_list = readFile(json_file)
    knn(1,1000,1001,1201,data_list,32)
    
