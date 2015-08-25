/**
 * A program Solves the Jump Peg puzzle and Water Puzzle using generic BFS solver.
 *
 * @version	$Id$
 * @author	Suresh Babu Jothilingam
 * @author	Pradeep Kumar Duvvur
 *
 * Revisions:
 *	$Log$
 */

#include<iostream>
#include<vector>
#include<queue>
#include<map>
#include<list>

using namespace std;
template<typename T,typename E>
/*
 *	Generic Solver Engine Class
 */
class Solver {

private:

    queue< T > q;

public:

    virtual T init(){}
    virtual bool isSolution( T config){}
    virtual void insertChildParent( T child, T parent){}
    virtual E getNextConfig( T config){}
    virtual void display(){}
    virtual void findParent( T solution){}

    void solverEngine(){
        bool isFound=false;
        T out;
        T config;
        T temp;
        E possibleConfigs;
        config=init();
        q.push(config);
        while(!q.empty()){
            temp=q.front();
            possibleConfigs=getNextConfig(temp);
            for(int i=0;i<possibleConfigs.size();i++){
                if(isSolution(possibleConfigs[i])){
                    insertChildParent(possibleConfigs[i],temp);
                    out=possibleConfigs[i];
                    isFound=true;
                    q.empty();
                }
                else{
                  q.push(possibleConfigs[i]);
                  insertChildParent(possibleConfigs[i],temp);
                }
            }
            q.pop();
        }
        if(isFound){
            findParent(out);
            display();
        }
        else{
            cout<<"No Possible Solution"<<endl;
        }
    }
};
/*
 *	Problem Class which uses the Solver class to solve the puzzle	
 */
class PegandHolePuzzle : public Solver < vector < vector <char> > ,  vector < vector < vector <char> > > > {

private:

    vector< vector <char> > initialConfig;
    vector< vector < vector <char> > > storeConfig;
    list<vector < vector <char> > >:: iterator l;
    typedef map< vector < vector <char> >,vector < vector <char> > > :: value_type pair;
    map< vector < vector <char> >,vector < vector <char> > > complete;
    list< vector < vector <char> > > output;
    int row,col;

public:

    vector < vector <char> > init(){
        int row,col,hrow,hcol;
        cout<<"Enter the ROW COL of the board"<<endl;
        cin>>row>>col;
        this->row=row;this->col=col;
        cout<<"Enter the hole coordinates"<<endl;
        cin>>hrow>>hcol;
        vector<char> temp;
        for(int i=0;i<col;i++)
            temp.push_back('+');
        for(int i=0;i<row;i++)
            initialConfig.push_back(temp);
        initialConfig[hrow][hcol]='.';
        return initialConfig;
    }

    bool isSolution(vector< vector<char> > config){
        int counter=0;
        for(int i=0;i<row;i++){
            for(int j=0;j<col;j++){
                    if(config[i][j]=='+'){
                        counter++;
                    }
            }
        }
        if(counter==1){
            return true;
        }
        else{
            return false;
        }
    }

    bool check(vector < vector <char> > config){
			int ch=0;
			for(int i=0;i<row;i++){
				for(int j=0;j<col;j++){
					if(config[i][j]=='.'){
						ch++;
					}
				}
			}
			if(ch==1){
				return true;
			}
			else{
				return false;
			}
		}

		/*
		 * move method to check if the jumps are possible and move the pegs
		 * @param	board 	the config
		 * @param	x,y the row and col coordinates
		 * @param	poss	vector to store the generated configs
		 */

    void nextMove(vector< vector <char> > config, vector < vector < vector <char> > > &configContainer, int x, int y){
        //down jump
            if(x<=(row-1)-2 && config[x+1][y]=='+' && config[x+2][y]=='+'){
                vector< vector <char> > temp=config;
                temp[x][y]='+';
                temp[x+1][y]='.';
                temp[x+2][y]='.';
                configContainer.push_back(temp);
                nextMove(temp,configContainer,x+2,y);
            }
        //up jump
            if(x>=2 && config[x-2][y]=='+' && config[x-1][y]=='+'){
                vector< vector <char> > temp=config;
                temp[x][y]='+';
                temp[x-1][y]='.';
                temp[x-2][y]='.';
                configContainer.push_back(temp);
                nextMove(temp,configContainer,x-2,y);
            }
        //left jump
            if(y>=2 && config[x][y-2]=='+' && config[x][y-1]=='+'){
                vector< vector <char> > temp=config;
                temp[x][y]='+';
                temp[x][y-1]='.';
                temp[x][y-2]='.';
                configContainer.push_back(temp);
                nextMove(temp,configContainer,x,y-2);
            }
        //right jump
            if(y<=(col-1)-2 && config[x][y+1]=='+' && config[x][y+2]=='+'){
                vector< vector <char> > temp=config;
                temp[x][y]='+';
                temp[x][y+1]='.';
                temp[x][y+2]='.';
                configContainer.push_back(temp);
                nextMove(temp,configContainer,x,y+2);
            }
    }
		/*
		 * getConfig method to get the config from the solve method in the solver class find the coordinates of the hole and
		 * send the config to the move method to get the possible configs
		 */
    vector < vector < vector <char> > > getNextConfig(vector < vector <char> > config){
        storeConfig.clear();
        for(int i=0;i<row;i++){
                for(int j=0;j<col;j++){
                    if(config[i][j]=='.'){
                        nextMove(config,storeConfig,i,j);
                    }
                }
            }
        return storeConfig;
    }

    void insertChildParent(vector< vector <char> > child, vector< vector <char> > parent){
        complete.insert(pair(child,parent));
    }

    void findParent(vector < vector <char> > solution){
        if(!check(solution)){
            output.push_front(solution);
            findParent(complete.find(solution)->second);
        }
    }

    void display(){
        output.push_front(initialConfig);
        for(l=output.begin();l!=output.end();l++){
            vector< vector <char> > config = *l;
            for(int i=0;i<row;i++){
                for(int j=0;j<col;j++){
                    cout<<config[i][j]<<" ";
                }
                cout<<endl;
            }
            cout<<endl;
        }
    }
};
/*
 *	Problem Class which uses the Solver class to solve the puzzle	
 */
class WaterBucketPuzzle : public Solver < vector <int>  ,  vector < vector <int> > >{

private:

    int noBuckets;
    int finalCapacity;
    vector<int> bucketSize;
    vector<int> initial;
    vector< vector <int> > possibleConfigs;
    vector< vector <int> > storeConfigs;
    list< vector <int> >:: iterator l;
    typedef map< vector <int> ,vector <int> > :: value_type pair;
    map< vector <int> ,vector <int> > complete;
    list< vector <int> > output;

public:

    vector<int> init(){
        int size,initialCapacity,bucketNo,noBuckets,finalCapacity;
        cout<<"Enter the number of buckets"<<endl;
        cin>>noBuckets;
        this->noBuckets=noBuckets;
        for(int i=0;i<noBuckets;i++){
            cout<<"Enter the capacity for bucket : "<<i<<endl;
            cin>>size;
            bucketSize.push_back(size);
            initial.push_back(0);
        }
        cout<<"Enter the intialCapacity of a bucket"<<endl;
        cout<<"Bucket no"<<endl;
        cin>>bucketNo;
        cout<<"Initial Capacity"<<endl;
        cin>>initialCapacity;
        cout<<"Enter the final Capacity"<<endl;
        cin>>finalCapacity;
        this->finalCapacity=finalCapacity;
        initial[bucketNo]=initialCapacity;
        return initial;
    }

    bool isSolution(vector <int> config){
        bool found=false;
        for(int i=0;i<config.size();i++){
            if(config[i]==finalCapacity){
                found=true;
                break;
            }
        }
        if(found){
            return true;
        }
        else{
            return false;
        }
    }

     bool check(vector<int> config){
			if(config==initial){
                return true;
			}
			else{
                return false;
			}
		}

    bool isPresent(vector <int> config){
        int check=0;
        for(int i=0;i<storeConfigs.size();i++){
            if(storeConfigs[i]==config){
                check=1;
            }
        }
        if(check==1){
            return true;
        }
        else{
            return false;
        }
    }
		/*
		 * full bucket function to full each bucket
		 */
    void fullBucket(vector <int> config,vector < vector <int> > &container){
        for(int bucketLoop=0;bucketLoop<config.size();bucketLoop++){
            if(config[bucketLoop]==0){
                vector<int> temp=config;
                temp[bucketLoop]=bucketSize[bucketLoop];
                if(!isPresent(temp)){
                    container.push_back(temp);
                    storeConfigs.push_back(temp);
                }
            }
        }
    }
		/*
		 * empty bucket function to empty each bucket
		 */
    void emptyBucket(vector <int> config,vector < vector <int> > &container){
        for(int bucketLoop=0;bucketLoop<config.size();bucketLoop++){
            if(config[bucketLoop]!=0){
                vector<int> temp=config;
                temp[bucketLoop]=0;
                if(!isPresent(temp)){
                    container.push_back(temp);
                    storeConfigs.push_back(temp);
                }
            }
        }
    }
		/*
		 * pour bucket function to transfer the water to another bucket
		 */
    void pourBucket(vector <int> config,vector < vector <int> > &container){
        int index;
        for(int bucketLoop=0;bucketLoop<config.size()-1;bucketLoop++){
            if(config[bucketLoop]!=0){
                index=bucketLoop;
                vector<int> temp=config;
                if(temp[index+1]<bucketSize[index+1] || temp[index+1]==0){
                    if(temp[index]<=(bucketSize[index+1]-temp[index+1])){
                        temp[index+1]=temp[index+1]+temp[index];
                        temp[index]=0;
                        if(!isPresent(temp)){
                            container.push_back(temp);
                            storeConfigs.push_back(temp);
                        }
                    }
                    else{
                        int remaining=temp[index]-(bucketSize[index+1]-temp[index+1]);
                        temp[index+1]=temp[index+1]+(bucketSize[index+1]-temp[index+1]);
                        temp[index]=remaining;
                        if(!isPresent(temp)){
                            container.push_back(temp);
                            storeConfigs.push_back(temp);
                        }
                    }
                }
            }
        }
    }
		/*
		 * getConfig method to get the config from the solve method in the solver class find the coordinates of the hole and
		 * send the config to the move method to get the possible configs
		 */
    vector < vector <int> > getNextConfig(vector <int> config){
            possibleConfigs.clear();
            fullBucket(config,possibleConfigs);
            emptyBucket(config,possibleConfigs);
            pourBucket(config,possibleConfigs);
            return possibleConfigs;
    }

    void insertChildParent(vector<int> child, vector<int> parent){
        complete.insert(pair(child,parent));
    }

    void findParent(vector<int> solution){
        if(!check(solution)){
            output.push_front(solution);
            findParent(complete.find(solution)->second);
        }
    }

    void display(){
	 cout<<"The solution is "<<endl;
        output.push_front(initial);
        for(l=output.begin();l!=output.end();l++){
            vector<int> config = *l;
             for(int i=0;i<config.size();i++){
                cout<<config[i]<<" ";
                }
            cout<<endl;
        }
    }
};

int main(){
    Solver < vector <int> , vector < vector <int> > > *swbp=new WaterBucketPuzzle;
    swbp->solverEngine();
    //Solver < vector < vector <char> > ,  vector < vector < vector <char> > > > *sphp=new PegandHolePuzzle;
    //sphp->solverEngine();
}
