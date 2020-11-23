# Genetic-Algorithms
This repository is all about solving some well known backtracking problems with the help of genetic algorithm.

## Introduction to Genetic Algorithm
According to the article from [towardsdatascience](https://towardsdatascience.com/introduction-to-genetic-algorithms-including-example-code-e396e98d8bf3#:~:text=A%20genetic%20algorithm%20is%20a,offspring%20of%20the%20next%20generation.), a genetic algorithm is a search heuristic that is inspired by Charles Darwin's theory of natural evolution. This algorithm reflects the process of natural selection where the fittest individuals are selected for reproduction in order to produce offspring of the next generation.

## How we solved the assignments using genetic algorithm

### Initializing the population
The intial population is randomply generated and we make sure that all the chromosomes in the population are distinct.

Here is the code of how we generated intial population in C++ in N-Queen assignment.
Creating Random Permutation
```cpp
/*
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
```

Intializing the population with unique random permutations.
```cpp
/*
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
```

### Fitness Function
For every chromosome in population, we calculated its fitness. For different assignments we used different fitness functions. But we always ensured that the fitness function must be monotonically increasing/decreasing as we reach the one of the possible solutions.

Here is one of the code snippet of calculating fitness function in N-Queen assignment. For every chromosome in population, we calculated the number of collisions in the chess board. And we declared fitness as (1/(collisions+0.01)). We added 0.01 to avoid the scenario where we get 0 collisions. This fitness function ensures that as number of collisions increases, the fitness will decrease. And as collisions can't be less than 0, this function is monotonic in nature.
```cpp
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
  fitness = 1/(collisions+0.01);
  return fitness; 
}
```

### Crossover
### Mutation
### Creating Offsprings
### Selecting best ones
### Repeating the process
