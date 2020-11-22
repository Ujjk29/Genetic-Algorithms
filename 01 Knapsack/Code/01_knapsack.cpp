#include<bits/stdc++.h>

using namespace std;

/*
Index of class methods:
1)variables and data structures
2)constructor
3)generate random representation of chromosome
4)population initialization
5)calculate average fitness
6)fitness
7)random number generator between 0 to 1
8)mutation 
9)crossover
10)parent selection
11)survivor selection
12)solve
*/

/*Class to solve knapsack problem using GA*/
class knapsackSolver{
public:
	//number of items
	int n;

	//capacity of knapsack
	int capacity;

	//population size will determine
	//how many combinations to consider
	//at a particular time
	int population_size;

	//Stores weight of all items
	vector<int>weight;

	//Stores value of all items
	vector<int>value;

	//Population consists of possible
	//combination of item selection.
	vector<vector<int>>population;

	//Maxx stores best fitness over every generations 
	int maxx=0;

	//ans vector stores the best result(chromosome)
	//encountered so far.
	vector<int>ans;

	/*
		Class constructor
		@param void
	*/
	knapsackSolver(){
		cin>>n;
		cin>>capacity;

		//input weight of items
		for(int i=0;i<n;i++){
			int inp;
			cin>>inp;
			weight.push_back(inp);
		}

		//input values of items
		for(int i=0;i<n;i++){
			int inp;
			cin>>inp;
			value.push_back(inp);
		}

		//input population size
		cin>>population_size;
		solve();
	}

	/*
		This method generates random chromosome
		and the chromosome is represented in binary format.
		@param void
		@return vector<int>
	*/
	vector<int> generate_random_representation(){
		srand(rand()%1000000000);
		vector<int>ans;
		int curr_weight=0;
		int start = rand()%100+1;
		if(start<=50){
			for(int i=0;i<n;i++){
				//Tossing a coin to determine 0/1
				int val = rand()%2;
				if(val==0){
					ans.push_back(0);
				}else if(val==1 && curr_weight+weight[i]<=capacity){
					ans.push_back(1);
					curr_weight+=weight[i];
				}else{
					ans.push_back(0);
				}
			}
		}else{
			for(int i=n-1;i>=0;i--){
				//Tossing a coin to determine 0/1
				int val = rand()%2;
				if(val==0){
					ans.push_back(0);
				}else if(val==1 && curr_weight+weight[i]<=capacity){
					ans.push_back(1);
					curr_weight+=weight[i];
				}else{
					ans.push_back(0);
				}
			}
			reverse(ans.begin(),ans.end());
		}
		return ans;
	}

	/*
		This method basically initializes whole population
		in such a way that there is no two similar chromosomes in the 
		whole population.
		@param void
		@return void
	*/
	void population_initialization(){
		set<vector<int>>population_set;
		for(int i=0;i<population_size;i++){
			vector<int>random_representation=generate_random_representation();
			while(population_set.find(random_representation)!=population_set.end()){
				random_representation=generate_random_representation();
			}
			population_set.insert(random_representation);
			population.push_back(random_representation);
		}
	}

	/*
		This function takes up all chromosomes and then calculate thier fitness
		using fitness function and return average.
		@param void
		@return long double
	*/
	long double calculate_avg_fitness(){
		long double sum=0;
		for(int i=0;i<population.size();i++){
			sum+=fitness(population[i]);
		}
		return (sum/(long double)population.size());
	}

	/*
		Fitness function takes up binary representation and returns the sum of value where
		c[i]==1 and this function is based on capacity restriction.
		@param vector<int> chromosome
		@return int
	*/
	int fitness(vector<int>chromosome){
		int ans=0;
		int curr_weight=0;
		for(int i=0;i<chromosome.size();i++){
			if(chromosome[i] && curr_weight+weight[i]<=capacity){
				ans+=chromosome[i]*value[i];
				curr_weight+=weight[i];
			}
		}
		return ans;
	}

	/*
		This function return random number between 0 and 1
		@param long double fmin
		@param long double fmax
		@return long double
	*/
	long double frand(long double fmin,long double fmax){
		srand(rand()%100000000);
		long double f = (long double)rand()/RAND_MAX;
		return fmin + f*(fmax-fmin);
	}

	/*
		It takes chromosome and based on limit, it mutates single bit.
		@param vector<int>& chromosome
		@return void
	*/
	void mutation(vector<int>&chromosome){
		srand(rand()%1000000000);
		double limit = (double)1/(double)n;
		for(int i=0;i<chromosome.size();i++){
			if(frand(0.0,1.0)>limit){
				chromosome[i]=!chromosome[i];
			}
		}
	}

	/*
		Simple crossover function in binary chromosome.
		@param vector<int> parent1
		@param vector<int> parent2
		@return pair<vector<int>,vector<int>>
	*/
	pair<vector<int>,vector<int>>crossover(vector<int>parent1,vector<int>parent2){
		vector<int>child1;
		vector<int>child2;

		int random_point = rand()%n;

		for(int i=0;i<random_point;i++){
			child1.push_back(parent1[i]);
			child2.push_back(parent2[i]);
		}

		for(int i=random_point;i<n;i++){
			child1.push_back(parent2[i]);
			child2.push_back(parent1[i]);
		}

		//returns two children
		return {child1,child2};
	}

	/*
		First 5 random chromosomes are selected then out that 2 best are taken.
		Crossover and mutation is applied.
		@param void
		@return pair<vector<int>,vector<int>>
	*/
	pair<vector<int>,vector<int> >parent_selection(){
		srand((rand()%100000000));

		vector<int>selected_parent_index;
		int count=0;

		while(count!=5){
			int index = rand()%population_size;
			selected_parent_index.push_back(index);
			count++;
		}

		vector<pair<int,int> >fitness_of_parents;

		for(int i=0;i<selected_parent_index.size();i++){
			fitness_of_parents.push_back({fitness(population[selected_parent_index[i]]),selected_parent_index[i]});
		}

		sort(fitness_of_parents.begin(),fitness_of_parents.end());

		vector<int>best_parent1 = population[fitness_of_parents[4].second];
		vector<int>best_parent2 = population[fitness_of_parents[3].second];

		pair<vector<int>,vector<int> >child = crossover(best_parent1,best_parent2);

		mutation(child.first);
		mutation(child.second);

		return {child.first,child.second};
	}

	/*
		For 20% of time we make children and then find out which 100% of population size are best
		and then select those chromosomes.
		@param void
		@return void
	*/
	void survivor_selection(){
		srand(rand()%100000000);

		vector<pair<int,vector<int> > >new_generation;

		for(int i=0;i<population_size/5;i++){
			pair<vector<int>,vector<int> >child = parent_selection();
			new_generation.push_back({fitness(child.first),child.first});
			new_generation.push_back({fitness(child.second),child.second});
		}

		for(int i=0;i<population_size;i++){
			new_generation.push_back({fitness(population[i]),population[i]});
		}

		sort(new_generation.begin(),new_generation.end(),[](pair<int,vector<int> >a,pair<int,vector<int> >b)->bool{return a.first>b.first;});

		for(int i=0;i<population_size;i++){
			population[i]=new_generation[i].second;
			if(new_generation[i].first>maxx){
				maxx=new_generation[i].first;
				ans=new_generation[i].second;
			}
		}
	}

	/*
		This is just to put all methods in one place.
		@param void
		@return void
	*/
	void solve(){
		population_initialization();
		for(int iteration=0;iteration<=20000;iteration++){
			std::this_thread::sleep_for(std::chrono::milliseconds(3));
			srand(rand()%100000000);
			long double avg = calculate_avg_fitness();
			cout<<"Current fitness of population : "<<avg<<endl;
			survivor_selection();
		}
		cout<<maxx<<endl;
		int curr_weight=0;
		int i=0;
		for(auto element:ans){
			if(element==1 && curr_weight+weight[i]<=capacity){
				cout<<"1 ";
				curr_weight+=weight[i];
			}else{
				cout<<"0 ";
			}
			i++;
		}
		cout<<endl;
	}
};

int main()
{
	srand(time(0));
	knapsackSolver knapsack;
}