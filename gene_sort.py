import numpy as np
import pandas as pd
from collections import defaultdict

CAP_file = pd.read_excel('CAP.xlsx')
CAP_file = np.array(CAP_file)

gene_list = defaultdict(int)

for i in CAP_file:
    AF = i[4]
    gene = i[12]
    is_shared_archaic = i[13]
    is_shared_CGO = i[14]
    if gene == -1: continue
    gene_list[gene] += 1

gene_list = sorted(gene_list.items(), reverse=True, key=lambda x:x[1])

output_file = 'gene_sort.txt'

for i in gene_list:
    with open(output_file, mode='a') as output:
        output.write(str(i) + "\n")