/*
Subset sum problem is to find subset of elements that are selected from a given set
whose sum adds up to a given number K. We are considering the set contains non-negative values.
It is assumed that the input set is unique (no duplicates are presented).
*/
#include<bits/stdc++.h>

using namespace std;

/*
Index:
1)variables
2)population initalization
3)print population
4)fitness
5)crossover
6)mutation
7)parent selection
8)comparator function
9)survivor selection
10)solve
11)public constructor
*/

class subset_sum_solver{
private:
	//number of elements in the array.
	int n;

	//target sum
	int k;

	//given elements of the array
	vector<int>arr;

	//population size will determine
	//how many combinations to consider
	//at a particular time
	int population_size;

	//Population consists of possible
	//combination of item selection.
	vector<vector<int> >population;

	//maxx stores max value of fitness
	long double avg_fitness,maxx=-1;

	//ans stores best answer(chomosome) till now.
	vector<int>ans;

	bool solution_achieved=false;

	/*
		Start with possible answers to the problem.
		Initialization of population is done in such 
		a way that no two chromosomes are same.
		@param int population_size
		@return void
	*/
	void population_initalization(int population_size){
		srand(rand()%1000000000);
		map<vector<int>,int>ump;
		while(population.size()!=population_size){
			vector<int>chromosome;
			int factor = rand()%1000000;
			for(int j=0;j<n;j++){
				if(rand()%(n*factor)>=((n*factor)/2)){
					chromosome.push_back(1);
				}else{
					chromosome.push_back(0);
				}
			}
			if(ump[chromosome]==0){
				population.push_back(chromosome);
				ump[chromosome]++;
			}
		}
	}

	/*
		@param void
		@return void	
	*/
	void print_population(){
		for(auto currentConfig:population){
			for(auto element:currentConfig){
				cout<<element<<" ";
			}
			cout<<endl;
		}
	}

	/*
		Fitness of function is calculated by 
		taking absolute difference between k and
		new sum.
		@param vector<int>chromosome
		@return long double
	*/
	long double fitness(vector<int>chromosome){
		int sum=0;
		long double epsilon = 0.01;
		for(int i=0;i<chromosome.size();i++){
			sum+=chromosome[i]*arr[i];
		}
		if(abs(k-sum)==0)
			solution_achieved=true;
		return (long double)1/(long double)(abs(k-sum)+epsilon);
	}

	/*
		simple binary chromosome crossover
		@param vector<int>parent1
		@param vector<int>parent2
		@return pair<vector<int>,vector<int>>
	*/
	pair<vector<int>,vector<int>>crossover(vector<int>parent1,vector<int>parent2){
		int crossover_point = rand()%n;
		vector<int>child1,child2;

		for(int i=0;i<crossover_point;i++){
			child1.push_back(parent1[i]);
			child2.push_back(parent2[i]);
		}

		for(int i=crossover_point;i<n;i++){
			child1.push_back(parent2[i]);
			child2.push_back(parent1[i]);
		}

		return {child1,child2};
	}

	/*
		mutation method that flips random index elements
		@param vector<int>& chromosome
		@return void
	*/
	void mutation(vector<int>&chromosome){
		srand(rand()%1000000000);
		int limit = n;
		int factor = rand()%1000000;
		for(int i=0;i<n;i++){
			if(rand()%(n*factor)>=((n*factor)/2)){
				chromosome[i]=!chromosome[i];
			}
		}
	}

	/*
		2 best out of random 5 are selected as parents
		@param void
		@return pair<vector<int>,vector<int>>
	*/
	pair<vector<int>,vector<int>>parent_selection(){
		srand(rand()%1000000000);
		vector<pair<long double,vector<int> > >selected_from_population;
		
		int it=0;
		while(it!=5){
			int ind = rand()%population_size;
			selected_from_population.push_back({fitness(population[ind]),population[ind]});
			it++;
		}

		sort(selected_from_population.begin(), selected_from_population.end());

		return {selected_from_population[3].second,selected_from_population[4].second};
	}

	/*
		@param pair<long double, vector<int>>a
		@param pair<long double, vector<int>>b
		@return bool
	*/
	static bool pair_double_vector_comparator(pair<long double,vector<int> >a,pair<long double,vector<int> >b){
		return a.first>b.first;
	}

	/*
		First parents are selected and then crossover is performed 
		and then mutation is done.
		@param void
		@return void
	*/
	void survivor_selection(){
		srand(rand()%1000000);
		vector<pair<long double,vector<int> > >new_generation;

		for(int i=0;i<population_size;i++){
			pair<vector<int>,vector<int> >parents = parent_selection();
			pair<vector<int>,vector<int> >children = crossover(parents.first,parents.second);
			mutation(children.first);
			mutation(children.second);
			new_generation.push_back({fitness(children.first),children.first});
			new_generation.push_back({fitness(children.second),children.second});
			new_generation.push_back({fitness(population[i]),population[i]});
		}

		sort(new_generation.begin(),new_generation.end(),pair_double_vector_comparator);

		long double fit_value=0;
		for(int i=0;i<population_size;i++){
			population[i]=new_generation[i].second;
			fit_value+=new_generation[i].first;
			if(maxx<=new_generation[i].first){
				maxx=new_generation[i].first;
				ans=new_generation[i].second;
			}
		}

		avg_fitness = fit_value/(long double)population_size;
	}

	/*
		Simulator of all the methods
		@param void
		@return void
	*/
	void solve(){
		for(int iteration=0;iteration<5000;iteration++){
			srand(rand()%1000000000);
			survivor_selection();
			cout<<"Current fitness of population: "<<avg_fitness<<endl;
			if(solution_achieved){
				cout<<"Solution achieved !!"<<endl;
				int sum=0;
				for(int i=0;i<n;i++){
					if(ans[i]){
						sum+=arr[i];
						cout<<arr[i]<<" ";
					}
				}
				cout<<endl;
				cout<<"Subset sum = "<<sum<<endl;
				cout<<endl;
				exit(0);
			}
		}
		cout<<endl;
		cout<<"Maximum fitness achieved : "<<maxx<<endl;
		int sum=0;
		int i=0;
		for(auto element:ans){
			if(element)
				cout<<arr[i]<<" ";
			sum+=element*arr[i];
			i++;
		}
		cout<<endl;
		cout<<"Subset sum = "<<sum<<endl;
		cout<<endl;
	}

public:
	subset_sum_solver(){
		srand(rand()%1000000000);
		cin>>n;
		cin>>k;

		for(int i=0;i<n;i++){
			int val;
			cin>>val;
			arr.push_back(val);
		}

		cin>>population_size;
		population_initalization(population_size);
		solve();
	}
};	

int main()
{
	srand(time(0));
	subset_sum_solver subset;
}