use rand::prelude::*;

const WIDTH: usize = 10;
const HEIGHT: usize = 10;

const TARGET: u16 = 0b1111_0000_0000_1111; 

#[derive(Clone, Copy, Debug)]
struct Cell {
    chromosome: u16,
    fitness: u16,
}
impl Cell {
    fn new(chromosome: u16) -> Self {
        Self {
            chromosome,
            fitness: 0,
        }
    }
    fn calculate_fitness(&mut self) {
        let errors = (self.chromosome ^ TARGET).count_ones() as u16;
        self.fitness = 16 - errors;
    }

    fn mutate(&mut self) {
        let rand_bit = rand::rng().random_range(0..16);
        self.chromosome ^= 1 << rand_bit;
    }
}

#[derive(Clone, Copy)]
struct Population {
    cells: [[Cell; WIDTH]; HEIGHT],
}

impl Population {
    fn new() -> Self {
        let mut cells = [[Cell::new(0); WIDTH]; HEIGHT];
        let mut rng = rand::rng();
        
        for i in 0..HEIGHT {
            for j in 0..WIDTH {
                let chromosome = rng.random_range(0..=u16::MAX);
                cells[i][j] = Cell::new(chromosome);
                cells[i][j].calculate_fitness();
            }
        }
        Self { cells }
    }

    fn get_neighbours(&self, i: i32, j: i32) -> Vec<&Cell> {
        let mut neighbours: Vec<&Cell> = vec![];
        let moves = [(1, 0), (-1, 0), (0, 1), (0, -1)];
        for (dy, dx) in moves {
            let next_i = i + dy;
            let next_j = j + dx;
            if next_i >= 0 && next_j >= 0 && next_i < HEIGHT as i32 && next_j < WIDTH as i32 {
                neighbours.push(&self.cells[next_i as usize][next_j as usize]);
            }
        }
        neighbours
    }
}

struct CGeneticAlgorithm {
    population: Population,
    next_population: Population,
}

impl CGeneticAlgorithm {
    fn new() -> Self {
        let population = Population::new();
        let next_population = population.clone();
        Self {
            population,
            next_population,
        }
    }

    fn run(&mut self, max_iter: u32) {
        let mut rng = rand::rng();

        for iter in 0..max_iter {
            let mut best_global_fitness = 0;
            let mut best_global_chromosome = 0;

            for i in 0..HEIGHT {
                for j in 0..WIDTH {
                    let current_cell = self.population.cells[i][j];
                    let neighbours = self.population.get_neighbours(i as i32, j as i32);

                    let best_neighbour = neighbours
                        .into_iter()
                        .max_by_key(|n| n.fitness)
                        .unwrap();

                    let child_chromosome = 
                        (current_cell.chromosome & 0xFF00) | (best_neighbour.chromosome & 0x00FF);

                    let mut child = Cell::new(child_chromosome);
                    if rng.random_bool(0.05) {
                        child.mutate();
                    }

                    child.calculate_fitness();

                    if child.fitness > best_global_fitness {
                        best_global_fitness = child.fitness;
                        best_global_chromosome = child.chromosome;
                    }

                    if child.fitness >= current_cell.fitness {
                        self.next_population.cells[i][j] = child;
                    } else {
                        self.next_population.cells[i][j] = current_cell;
                    }
                }
            }

            self.population = self.next_population.clone();

            if iter % 10 == 0 || best_global_fitness == 16 {
                println!(
                    "Generation {:>3} | Best fitness: {}/16 | Best chromosome: {:#018b}", 
                    iter, best_global_fitness, best_global_chromosome
                );
                if best_global_fitness == 16 {
                    println!("\nSolution found at {} iteration.", iter);
                    println!("Target:   {:#018b} ({:#06X})", TARGET, TARGET);
                    println!("Obtained: {:#018b} ({:#06X})", best_global_chromosome, best_global_chromosome);
                    break;
                }
            }
        }
    }
}

fn main() {
  
    let mut cga = CGeneticAlgorithm::new();
    cga.run(500); 
}
