#include<bits/stdc++.h>

using namespace std;

/*
Index:
1)variables
2)constructor
3)population initializer
4)generate random chromosome
5)fitness function
6)crossover
7)mutation 
8)random number generator
9)parent selection
10)survivor selection
11)fitness of all population
12)average fitness of current generation
13)solve method
*/

class travelling_salesman_solver{
private:
	//number of vertices
	int n;

	//population size will determine
	//how many combinations to consider
	//at a particular time
	int population_size;
	vector<vector<int>>graph;

	//Population consists of possible
	//combination of item selection.
	vector<vector<int>>population;

	//stores the fitness of the resulted chromosome
	vector<int>fitness_of_ith_chromosome;

	// minn contains minimum fitness
	int minn=INT_MAX;

	//ans contains best chromosome over all generations.
	vector<int>ans;

	/*
		generate population in such a away 
		that all chromosomes are distinct
		@param int size
		@return void
	*/
	void population_initialization(int size){
		set<vector<int>>population_set;
		for(int i=0;i<size;i++){
			vector<int>random_permutation = genrate_random_permutation(n);
			while(population_set.find(random_permutation)!=population_set.end()){
				random_permutation=genrate_random_permutation(n);
			}
			population.push_back(random_permutation);
			population_set.insert(random_permutation);
		}
	}

	/*
		generate random permutation based chromosome
		@param int n
		@return vector<int>
	*/
	vector<int> genrate_random_permutation(int n){
		vector<int>random_permutation;
		for(int i=1;i<=n;i++){
			random_permutation.push_back(i);
		}
		shuffle(random_permutation.begin(),random_permutation.end(),default_random_engine(rand()%100000000));
		return random_permutation;
	}

	/*
		fitness of tsp is defined as time taken to end trip.
		so to minimize the fitness function is our objective.
		@param vector<int> chromosome
		@return int
	*/
	int fitness_evaluation(vector<int>chromosome){
		int fitness=0;
		for(int i=0;i<n;i++){
			fitness+=graph[chromosome[i]-1][chromosome[(i+1)%n]-1];
		}
		return fitness; 
	}

	/*
		permutaion crossover.
		@param vector<int> parent1
		@param vector<int> parent2
		@return pair<vector<int>,vector<int>>		
	*/
	pair<vector<int>,vector<int> >crossover(vector<int>parent1,vector<int>parent2){
		// two children.
		vector<int>child1;
		vector<int>child2;

		unordered_map<int,int>ump;
		for(int i=0;i<n/2;i++){
			child1.push_back(parent1[i]);
			ump[parent1[i]]=1;
		}

		int count=0,it=n/2,rem = n-it;
		while(count!=(rem)){
			it = it%n;
			if(ump[parent2[it]]){
				it++;
			}else{
				child1.push_back(parent2[it]);
				count++;
				it++;
			}
		}

		ump.clear();
		for(int i=0;i<n/2;i++){
			child2.push_back(parent2[i]);
			ump[parent2[i]]=1;
		}

		count=0,it=n/2;
		while(count!=(rem)){
			it = it%n;
			if(ump[parent1[it]]){
				it++;
			}else{
				child2.push_back(parent1[it]);
				count++;
				it++;
			}
		}

		//returns two children
		return {child1,child2};
	}

	/*
		mutation crossover.
		@param vector<int>& v
		@return void		
	*/
	void mutation(vector<int>&v){
		long double probability = 1/(long double)n;
		//frand generates random number in between 0 and 1
		long double random = frand(0.0,1.0);
		if(random>probability){
			srand(rand()%100000000);

			// a and b are two distinct random number in permutation
			// such that swap happens correctly.
			int a=rand()%n;
			int b=rand()%n;
			while(b==a){
				b=rand()%n;
			}
			swap(v[a],v[b]);
		}
	}	

	/*
		random number geneartor between 0 and 1.
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
		parent selection. Best 2 out of random 5 from population is used.
		@param void
		@return pair<vector<int>,vector<int>>
	*/
	pair<vector<int>,vector<int> >parent_selection(){
		srand((rand()%100000000));
		//This data structure stores 5 indices of selected parent.
		vector<int>selected_parent_index;
		int count=0;

		while(count!=5){
			int index = rand()%population_size;
			selected_parent_index.push_back(index);
			count++;
		}

		//this stores fitness value of parents and index of that parent.
		vector<pair<int,int> >fitness_of_parents;

		for(int i=0;i<selected_parent_index.size();i++){
			fitness_of_parents.push_back({fitness_of_ith_chromosome[selected_parent_index[i]],selected_parent_index[i]});
		}

		// we sort according to fitness.
		sort(fitness_of_parents.begin(),fitness_of_parents.end());

		// best two parents are selected that have highest fitness.
		vector<int>best_parent1 = population[fitness_of_parents[4].second];
		vector<int>best_parent2 = population[fitness_of_parents[3].second];

		// we crossover two best parents and get two children.
		pair<vector<int>,vector<int> >child = crossover(best_parent1,best_parent2);

		// mutation are used over both children.
		mutation(child.first);
		mutation(child.second);

		return {child.first,child.second};
	}

	/*
		survivor selection is based on replace worst
		@param void
		@return void
	*/
	void survivor_selection(){
		srand(rand()%100000000);

		// new generation stores fitness value and chromosome
		vector<pair<int,vector<int> > >new_generation;

		for(int i=0;i<population_size/5;i++){
			// children are selected and pushed into new generation.
			pair<vector<int>,vector<int> >child = parent_selection();
			new_generation.push_back({fitness_evaluation(child.first),child.first});
			new_generation.push_back({fitness_evaluation(child.second),child.second});
		}

		for(int i=0;i<population_size;i++){
			new_generation.push_back({fitness_of_ith_chromosome[i],population[i]});
		}

		sort(new_generation.begin(),new_generation.end(),[](pair<int,vector<int> >a,pair<int,vector<int> >b)->bool{return a.first>b.first;});

		// now only those individuals are seleted which are fitter.
		for(int i=0;i<population_size;i++){
			population[i]=new_generation[i].second;
			if(new_generation[i].first<minn){
				minn=new_generation[i].first;
				ans=new_generation[i].second;
			}
		}
	}

	/*
		@param void
		@return void
	*/
	void calculate_fitness_of_all_chromosomes(){
		fitness_of_ith_chromosome.clear();
		for(int i=0;i<population_size;i++){
			fitness_of_ith_chromosome.push_back(fitness_evaluation(population[i]));
		}		
	}

	/*
		Calculates fitness average of population
		@param void
		@return long double
	*/
	long double fitness_average_of_population(){
		long double sum=0;
		for(int i=0;i<population_size;i++){
			long double fitness_value = fitness_of_ith_chromosome[i];
			sum+=fitness_value;
		}
		return (long double)sum/(long double)population_size;
	}
	
public:
	/*
		constructor to initialize all variables.
		@param void
	*/
	travelling_salesman_solver(){
		cout<<"Enter number of vertices."<<endl;
		cin>>n;
		
		cout<<"Enter the graph in matrix format."<<endl;
		for(int i=0;i<n;i++){
			vector<int>temp;
			for(int j=0;j<n;j++){
				int val;
				cin>>val;
				if(i==j)val=0;
				temp.push_back(val);
			}
			graph.push_back(temp);
		}

		cout<<"Enter population size."<<endl;
		cin>>population_size;
	}

	/*
		@param void
		@return void
	*/
	void solve(){
		population_initialization(population_size);
		for(int iteration=0;iteration<=10000;iteration++){
			srand(rand()%1000000);
			calculate_fitness_of_all_chromosomes();
			long double avg = fitness_average_of_population();
			cout<<"Current fitness average of population : "<<avg<<endl;
			survivor_selection();
		}
		cout<<minn<<endl;
		for(auto element:ans){
			cout<<element<<" ";
		}
		cout<<endl;
	}
};

int main()
{
	srand(time(0));
	travelling_salesman_solver travelling_salesman;
	travelling_salesman.solve();
}
