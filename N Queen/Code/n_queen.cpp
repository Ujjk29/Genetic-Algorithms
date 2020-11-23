#include<bits/stdc++.h>

using namespace std;

/*
Index of class methods:
1)  constructor
2)  solution board
3)  population initialization
4)  generate random permutaion
5)  collision calculator
6)  fitness evaluation
7)  crossover
8)  mutation
9)  frand
10) parent selection
11) survivor selection
12) calculate fitness of all chromosomes
13) fitness average of population
14) solve
*/

class n_queen_solver{
private:
	//number of queens.
	int n;

	//population size will determine
	//how many combinations to consider
	//at a particular time
	int population_size;

	bool solution_achieved=false;

	//To store the solution chromosome if solution is achieved.
	vector<int>solution;

	//Matrix representation of board in form of dynamic array aka vector.
	vector<vector<int>>board;

	//Population represents many chromosomes.
	//Each chromosome is of length n and all those chromosomes are stored in
	//population in vector<vector<int> > form.
	vector<vector<int>>population;

	//stores the fitness of the resulted chromosome
	vector<long double>fitness_of_ith_chromosome;

	/*
		Print the chess board
		@param void
		@return void
	*/
	void solution_board(){
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				cout<<board[i][j]<<" ";
			}
			cout<<endl;
		}
	}

	/*
		This method stores all distinct random 
		chromosomes in population matrix
		@param int size
		@return void
	*/
	void population_initialization(int size){
		set<vector<int>>population_set;
		for(int i=0;i<size;i++){
			//generating random permutation.
			vector<int>random_permutation = genrate_random_permutation(n);
			while(population_set.find(random_permutation)!=population_set.end()){
				random_permutation=genrate_random_permutation(n);
			}
			population.push_back(random_permutation);
			population_set.insert(random_permutation);
		}
	}

	/*
		This method generates random permutation of chromosome.
		@param int n
		@return vector<int>
	*/
	vector<int> genrate_random_permutation(int n){
		//vector to store random permutation.
		vector<int>random_permutation;
		//for loop used to generate distinct random numbers.
		for(int i=1;i<=n;i++){
			random_permutation.push_back(i);
		}
		shuffle(random_permutation.begin(),random_permutation.end(),default_random_engine(rand()%100000000));
		return random_permutation;
	}

	/*
		This function calculates collisions for a particular board.
		@param vector<int> chromosome
		@return int
	*/
	int collision_calculator(vector<int>chromosome){
		/*
			2D representation of chromosomes.
			Queen vector shows positions on board.
		*/
		vector<pair<int,int> >queen;
		for(int i=0;i<n;i++){
			queen.push_back({chromosome[i]-1,i});
		}

		//Finding number of collisions.
		long double collisions=0;
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				if(i!=j){
					//line collision check
					if(queen[i].first==queen[j].first || queen[i].second==queen[j].second){
						collisions++;
						continue;
					}
					//diagonal collision check
					int diffx = abs(queen[i].first-queen[j].first);
					int diffy = abs(queen[i].second-queen[j].second);
					if(diffx==diffy){
						collisions++;
					}
				}
			}
		}
		return collisions;
	}

	/*
		function to evaluate fitness
		@param vector<int> chromosome
		@return long double
	*/
	long double fitness_evaluation(vector<int>chromosome){
		int collisions = collision_calculator(chromosome);
		long double fitness,epsilon=0.01;
		//fitness = 1/(collision+0.01) this means we have to maximize fitness.
		fitness = 1/(collisions+0.01);
		return fitness; 
	}

	/*
		crossover takes two parents , and does crossover 
		and returns two children. Crossover uses midpoint as crossover point.
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
		Swap Mutation is used and it takes reference of vector 
		and changes it if random probability is greater than 
		fixed probability = 1/n
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
		frand fuunction generates random number between 0 and 1.
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
		parent selection -> used tournament selection 
		(best 2 out of random 5 without repition)
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
		vector<pair<long double,int> >fitness_of_parents;

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
		vector<pair<long double,vector<int> > >new_generation;

		for(int i=0;i<population_size/5;i++){
			// children are selected and pushed into new generation.
			pair<vector<int>,vector<int> >child = parent_selection();
			new_generation.push_back({fitness_evaluation(child.first),child.first});
			new_generation.push_back({fitness_evaluation(child.second),child.second});
		}

		for(int i=0;i<population_size;i++){
			new_generation.push_back({fitness_of_ith_chromosome[i],population[i]});
		}

		sort(new_generation.begin(),new_generation.end(),[](pair<long double,vector<int> >a,pair<long double,vector<int> >b)->bool{return a.first>b.first;});

		// now only those individuals are seleted which are fitter.
		for(int i=0;i<population_size;i++){
			population[i]=new_generation[i].second;
		}
	}

	/*
		Calculates fitness of all chromosomes 
		currently available in population
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
			if(collision_calculator(population[i])==0){
				solution_achieved=true;
				solution = population[i];
			}
		}
		return (long double)sum/(long double)population_size;
	}

public:
	/*
		Constructor for n queen class.
		Inputs are value of n and population size
		@param int n
		@param int population_size
	*/
	n_queen_solver(int n,int population_size){
		this->n=n;
		this->population_size=population_size;
		/*Generating initial boolean matrix for board representation.*/
		for(int i=0;i<n;i++){
			vector<int>temp;
			for(int j=0;j<n;j++){
				temp.push_back(0);
			}
			board.push_back(temp);
		}
		/*Method to initalize population with population_size chromosomes*/
		population_initialization(population_size);
	}

	/*
		This is the important funtion in this class
		Simulates all the working.
		First calculate fitness of population.
		Then if solution is achieved then print solution
		else it selects survivor.
		@param void
		@return void
	*/
	void solve(){
		for(int it=0;it<=50000;it++){
			srand(time(0)%100000000);
			calculate_fitness_of_all_chromosomes();
			long double avg = fitness_average_of_population();
			cout<<"Current fitness average of population : "<<avg<<endl;
			if(solution_achieved){
				cout<<"Solution achieved at iteration : "<<it<<endl;
				for(int i=0;i<solution.size();i++){
					board[solution[i]-1][i]=1;
				}
				solution_board();
				return;
			}
			survivor_selection();
		}
		cout<<"Solution not achieved !!"<<endl;
	}
};

int main()
{
	int n;
	cout<<"Enter value of N"<<endl;
	cin>>n;
	int population_size;
	cout<<"Enter population size"<<endl;
	cin>>population_size;
	n_queen_solver n_queen(n,population_size);
	n_queen.solve();
}