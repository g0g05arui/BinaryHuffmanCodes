#include <iostream>
#include <set>
#include <queue>
#include <unordered_map>

class Label{
protected:
    std::string _name;
    double _probability;
    int _id;
public:
    Label():_name(""),_probability(0),_id(0){}
    Label(std::string name,double probability,int id=0):_name(name),_probability(probability),_id(id){}
    const std::string & name()const{
        return _name;
    }
    double probability()const{
        return _probability;
    }
    int id()const{
        return _id;
    }
    bool operator > (const Label & other)const{
        if(other.probability()==_probability)
            return _id>other.id();
        return _probability>other.probability();
    }
    friend class BinaryHuffmanTree;
};

class BinaryHuffmanTree{
private:
    std::priority_queue<Label,std::vector<Label>,std::greater<Label> > labels;
    std::vector<Label> newValues;
    std::unordered_map<std::string,std::string> _representation;
    bool _isValid=true;
    static double constexpr EPS=1e-7;
    int currentId=0;
    int newId(){
        return currentId++;
    }
    std::vector<std::pair<int,int> >_graph;
    static int constexpr EMPTY=-1;
   void dfs(int id,std::string prefix){
        if(id>=0 && id<newValues.size())
            _representation[newValues[id].name()]=prefix;
        if(_graph[id].first!=EMPTY)
            dfs(_graph[id].first,prefix+"0");
        if(_graph[id].second!=EMPTY)
            dfs(_graph[id].second,prefix+"1");
    }
    std::vector<Label> concatLabels(const std::vector<Label>& oldLabels,const std::vector<Label>&labels){
        std::vector<Label>aux;
        for(Label l1:oldLabels)
            for(Label l2:labels)
                aux.push_back(Label(l1.name()+l2.name(),l1.probability()*l2.probability()));
        return aux;
    }

public:
    BinaryHuffmanTree(const std::vector<Label> &values,int wordLength=1){
        double probability=0;
        for(Label l:values){
            newValues.push_back(l);
            probability+=l.probability();
        }
        for(int L=1;L<wordLength;L++)
            newValues=concatLabels(newValues,values);
        for(Label & l:newValues){
            l._id=newId();
            labels.push(l);
            _graph.push_back({EMPTY,EMPTY});
        }
        _isValid=(probability-1>-EPS && probability-1<EPS);
    }

    bool isValid(){
        return _isValid;
    }

    void build() {
        while (!labels.empty() && labels.size() != 1) {
            Label l1, l2, newLabel;
            l1 = labels.top();
            labels.pop();
            l2 = labels.top();
            labels.pop();
            newLabel = Label("", l1.probability() + l2.probability(), newId());
            labels.push(newLabel);
            int newLabelId = newLabel.id();
            _graph.push_back({l1.id(), l2.id()});
        }
    }

    const std::unordered_map<std::string,std::string>& representation(){
        int id=_graph.size()-1;
        if(!_representation.empty())
            return _representation;
        dfs(id,"");
        return _representation;
    }

};

int main(){
    Label l1=Label("A",0.1),l2=Label("B",0.1),l3=Label("C",0.2),l4=Label("D",0.2),l5=Label("E",0.4);
    std::vector<Label> labels;
    labels.push_back(l1);
    labels.push_back(l2);
    labels.push_back(l3);
    labels.push_back(l4);
    labels.push_back(l5);
    BinaryHuffmanTree bht=BinaryHuffmanTree(labels,2);
    bht.build();
    auto x=bht.representation();
    for(auto y:x)
        std::cout<<y.first<<" "<<y.second<<'\n';
    return 0;
}
