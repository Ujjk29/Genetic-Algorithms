# Genetic-Algorithms
This repository is all about solving some well known backtracking problems with the help of genetic algorithm. Here is the list of problems that we solved using genetic algorithm:

1. [01 Knapsack](../../tree/main/01%20Knapsack)
2. [N Queen](../../tree/main/N%20Queen)
3. [Subset Sum](../../tree/main/Subset%20Sum)
4. [Travelling Salesman](../../tree/main/Travelling%20Salesman)

## Introduction to Genetic Algorithm
According to the article from [towardsdatascience](https://towardsdatascience.com/introduction-to-genetic-algorithms-including-example-code-e396e98d8bf3#:~:text=A%20genetic%20algorithm%20is%20a,offspring%20of%20the%20next%20generation.), a genetic algorithm is a search heuristic that is inspired by Charles Darwin's theory of natural evolution. This algorithm reflects the process of natural selection where the fittest individuals are selected for reproduction in order to produce offspring of the next generation.

## How we solved the assignments using genetic algorithm

### 1. Initializing the population
The intial population is randomply generated and we make sure that all the chromosomes in the population are distinct. Here is the code of how we generated intial population in C++ in N-Queen assignment.

Creating Random Permutation
```cpp
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

### 2. Fitness Function
For every chromosome in population, we calculated its fitness. For different assignments we used different fitness functions. But we always ensured that the fitness function must be monotonically increasing/decreasing as we reach the one of the possible solutions.

Here is one of the code snippet of calculating fitness function in N-Queen assignment. For every chromosome in population, we calculated the number of collisions in the chess board. And we declared fitness as ```(1/(collisions+0.01))```. We added 0.01 to avoid the scenario where we get 0 collisions. This fitness function ensures that as number of collisions increases, the fitness will decrease. And as collisions can't be less than 0, this function is monotonic in nature.
```cpp
int collision_calculator(vector<int>chromosome){
  vector<pair<int,int> >queen;
  for(int i=0;i<n;i++){
    queen.push_back({chromosome[i]-1,i});
  }
  long double collisions=0;
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      if(i!=j){
        if(queen[i].first==queen[j].first || queen[i].second==queen[j].second){
          collisions++;
          continue;
        }
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
```

```cpp
long double fitness_evaluation(vector<int>chromosome){
  int collisions = collision_calculator(chromosome);
  long double fitness,epsilon=0.01;
  fitness = 1/(collisions+0.01);
  return fitness; 
}
```

### 3. Crossover
For crossover we selected two parents and created two offsprings. For both the binary chromosome and permutation chromosome, we used one-point crossover method. Here is one of the crossover functions that we used in solving 0-1 Knapsack problem in c++.

```cpp
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
  return {child1,child2};
}
```

### 4. Mutation
We do mutation to the resultant offspring to ensure that fitness of the next generation doesn't become constant. Here is the code snippet of doing mutation on the binary chromosome.

```cpp
void mutation(vector<int>&chromosome){
  srand(rand()%1000000000);
  double limit = (double)1/(double)n;
  for(int i=0;i<chromosome.size();i++){
    if(frand(0.0,1.0)>limit){
      chromosome[i]=!chromosome[i];
    }
  }
}
```

### 5. Creating Offsprings
For every 5 parents, we select best 2 parents and they are used to make new offsprings using crossover and mutation. Here is code snippet, selecting 2 best parents out of 5, and then performing crossover to get offsprings and finally applying mutation on it.

```cpp
pair<vector<int>,vector<int> >parent_selection(){
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
```

### 6. Selecting best ones
We now combine parents and generated offsprings in the same pool and select best ones by calculating the fitness function. For eg, the size of initial population is 100, then 20 offsprings are added. Now out of 120, we select best 100 on the basis of their fitness. Here is the code snippet of selecting survivors.

```cpp
void survivor_selection(){
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
```

### 7. Repeating the process
We repeat this process several times. The best number of iterations depends on the size of the intial population that you start with. For 100 population size, we use 5,000 to 10,000 iterations. 

## Results
For large test cases of N-Queen problem and Travelling salesman problem, we got results within few minutes using genetic algorithm as compared to the normal backtracking approach that would have taken time more than the age of universe.

## Room for improvement
There are fixed number of chromosomes for the given problem. And sometimes, we calculate the fitness of the chromosomes that we have already calculated. So we can use map to store chromosome and its fitness value. This will increase space complexity, but will reduce the time complexity drastically.
