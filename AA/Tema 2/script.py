from typing import List
from random import random as rand
import pathlib as pl
import os
from matplotlib import pyplot as plt

# Vrei sa citesti inputul din fisierul de intrare ?
USE_INPUT_FILE: bool = True
# Pentru cate generatii vrem sa afisam mai multe detalii
NUMAR_GENERATII_OUTPUT_DETALIAT = 1

INPUT_FILE = os.path.join(str(pl.Path(__file__).parent.absolute()), 
                          "date_intrare.txt")
OUTPUT_FILE = os.path.join(str(pl.Path(__file__).parent.absolute()), 
                          "alg_evolutie.txt")
# Decide comportamentul algoritmului genetic

# selectia (daca se aleg mai multe strategii, sunt folosite in aceasta ordine,
# iar ultimele 3 sunt ciclate pana ajungem la numarul dorit de indivizi)
# Iau sau nu cel mai bun de fiecare data ?
SEL_ELITISTA = 1
# Urmatoarele 3 in a loop
SEL_PROPORTIONALA = 0
SEL_TURNEU = 0
SEL_ORDONARE = 0

# daca folosim operatorul de incrucisare
ALG_INCRUCISARE = 1

# daca folosim operatorul de mutatie
# Recomandat sa fie folosita exact 1 dintre variante
ALG_MUTATIE_RARA = 1
ALG_MUTATIE_DEASA = 0

# Constantele problemei
DIM_POP = 0
DOM_A, DOM_B = 0, 0
PRECIZIE = 0  # O putere de 10, -->> 10^PRECIZIE
COEF_A, COEF_B, COEF_C = 0, 0, 0
P_REC, P_MUT = 0, 0
EPOCHS = 0


# Constante pentru plotting
SECONDARY_PERCENT = 15  # Top cat % urmaresc performanta medie si cea mai proasta 
                        # pentru fiecare generatie

def parse_input_data():
    global DIM_POP
    global DOM_A, DOM_B
    global PRECIZIE
    global COEF_A, COEF_B, COEF_C
    global P_REC, P_MUT
    global EPOCHS
    with open(INPUT_FILE, "r") as f:
        DIM_POP = int(f.readline())
        DOM_A, DOM_B = map(int, f.readline().split())
        PRECIZIE = int(f.readline())
        COEF_A, COEF_B, COEF_C = map(int, f.readline().split())
        P_REC, P_MUT = map(float, f.readline().split())
        EPOCHS = int(f.readline())

class Individ:
    def __init__(self, bits: List[int]):
        self.bits = bits
    
    # Modifica Individul 
    def rare_mutation(self, probability: float = 1.0):



def Simuleaza(precizie: int, epochs: int, out_file = None):
    print("Simulam cu precizie " + str(precizie) + " pentru " +
           str(epochs) + " generatii")
    

def Run_different():
    start_prec, end_prec = 1, 10
    nr_epochs = 10000
    results = [Simuleaza(pr, nr_epochs) for pr in range(start_prec, end_prec + 1)]
    # Some plotting

def main():
    if USE_INPUT_FILE:
        parse_input_data()
    # Rulam simularea normala
    Simuleaza(PRECIZIE, OUTPUT_FILE)
    
    # Am comparat mai multe rulari cu precizii diferite
    # Run_different_precisions()
    
if __name__ == "__main__":
    main()
