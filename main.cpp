#include <iostream>
#include <vector>
using namespace std;


class handNode
{
public:

    int value = 0;
    int numberOfSiblings;
    double parentProbability;
    double selfProbability;
    bool finalHand;
    vector<int> cardVector;
    int openCardValue;

public:

    handNode(int nOSParam, double pPParam, vector<int> cardVectorParam)
    {
        numberOfSiblings = nOSParam;
        parentProbability = pPParam;
        selfProbability = static_cast<double>(1.0/numberOfSiblings)*parentProbability;
        cardVector = cardVectorParam;

        for(int i=0; i<cardVector.size(); i++)
        {
            value = value + cardVector[i];
        }

        if(value>=18)
        {
            finalHand = true;
        }
        else
        {
            finalHand = false;
        }
    }
};

vector<int> eraseFunction(int number, vector<int> vector)
{
    for(int i=0; i<vector.size(); i++)
    {
        if(vector[i]==number)
        {
            vector.erase(vector.begin()+i);
            break;
        }
    }
    return vector;
}

vector<handNode> startingFunction(vector<int> knownDeck, int playerOpenCardValue, vector<handNode> handNodeVector)
{
    vector<int> tempVector;

    for(int i=0; i<knownDeck.size(); i++)
    {
        tempVector.push_back(playerOpenCardValue);
        tempVector.push_back(knownDeck[i]);
        handNodeVector.emplace_back(knownDeck.size(),1.0,tempVector);
        tempVector.clear();
    }

    return handNodeVector;
}

void assistantFunction(vector<handNode> &handNodeVector, vector<int> knownDeck, int lastRunLastNodeID, int lastRunFirstNodeID, int limit)
{
    vector<int> originalVector;
    originalVector = knownDeck;
    vector<int> tempVector;
    int nodesCreatedThisRun = 0;
    int firstNodeID;
    int lastNodeID;

    for(int i=lastRunFirstNodeID; i<=lastRunLastNodeID; i++)
    {
        if(handNodeVector[i].value<limit)
        {
            knownDeck = originalVector;

            // Remove the cards that CAN'T be drawn, starting from the last element
            for (auto it = handNodeVector[i].cardVector.begin() + 1; it != handNodeVector[i].cardVector.end(); ++it)
            {
                int cardThatWillBeRemoved = *it; // Get the current card (excluding the open card value)
                knownDeck = eraseFunction(cardThatWillBeRemoved, knownDeck);
            }

            for(int create=0; create<knownDeck.size(); create++)
            {
                tempVector = handNodeVector[i].cardVector;
                tempVector.push_back(knownDeck[create]);
                handNodeVector.emplace_back(knownDeck.size(),handNodeVector[i].selfProbability,tempVector);
                tempVector.clear();
                if(nodesCreatedThisRun==0 && create==0)
                {
                    firstNodeID = handNodeVector.size()-1;
                }
                nodesCreatedThisRun++;
            }
            knownDeck = originalVector;
        }
    }
    knownDeck = originalVector;
    lastNodeID = handNodeVector.size()-1;

    if(nodesCreatedThisRun!=0)
    {
        assistantFunction(handNodeVector,knownDeck,lastNodeID,firstNodeID,limit);
    }

}

double playerKeepsDrawing(vector<int> knownDeck, int playerOpenCardValue, vector<handNode> &handNodeVectorParameter, int satisfactionValue, int gladosHandValue)
{
    vector<handNode> handNodeVector;
    handNodeVector = startingFunction(knownDeck,playerOpenCardValue,handNodeVectorParameter);
    assistantFunction(handNodeVector,knownDeck,knownDeck.size()-1,0,satisfactionValue);

    for(int i=0; i<handNodeVector.size(); i++)
    {
        cout<<"i: "<<i<<endl;
        cout<<"v: "<<handNodeVector[i].value<<endl;
        cout<<endl;
    }

    double gladosHavingSuperiorHandProb = 0;
    double gladosHavingInferiorHandProb = 0;
    double equalHandProb = 0;


    for(int i=0; i<handNodeVector.size(); i++)
    {
        if(handNodeVector[i].finalHand)
        {
            if(handNodeVector[i].value>gladosHandValue && handNodeVector[i].value<=21)
            {
                gladosHavingInferiorHandProb = gladosHavingInferiorHandProb + handNodeVector[i].selfProbability;
            }
            else if(handNodeVector[i].value<gladosHandValue || handNodeVector[i].value>21)
            {
                gladosHavingSuperiorHandProb = gladosHavingSuperiorHandProb + handNodeVector[i].selfProbability;
            }
            else
            {
                equalHandProb = equalHandProb + handNodeVector[i].selfProbability;
            }
        }
    }

    cout<<"sup: "<<gladosHavingSuperiorHandProb<<endl;
    cout<<"inf: "<<gladosHavingInferiorHandProb<<endl;
    cout<<"tie: "<<equalHandProb<<endl;

    return gladosHavingSuperiorHandProb;

}

int main()
{
    vector<int> testVector;
    vector<handNode> handNodeVector;
    double prob;

    testVector.push_back(6);
    testVector.push_back(8);
    testVector.push_back(10);
    testVector.push_back(2);
    testVector.push_back(2);

    int playerOpenCard = 10;
    int satisfaction = 18;
    int gladosHandValue = 18;

   prob = playerKeepsDrawing(testVector,playerOpenCard,handNodeVector,satisfaction,gladosHandValue);
   cout<<prob<<endl;


}
