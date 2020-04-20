import sys, os
from Bio import SeqIO, Align
import numpy as np

path = "./orthologs/"

human_index = []
human_seq = []

# 各 FASTA ファイルで Homo sapiens は何番目に記述されているか調べる
for i in os.listdir(path):
    seq_cnt = 0
    for record in SeqIO.parse(path + i, "fasta"):
        id = record.id
        desc = record.description
        seq = record.seq

        if "Homo sapiens" in desc:
            human_index.append(seq_cnt)
            human_seq.append(seq)

        seq_cnt += 1

# アライメントの設定
aligner = Align.PairwiseAligner()
aligner.mode = "global"
aligner.match_score = 1
aligner.mismatch_score = -0.5
aligner.open_gap_score = -2
aligner.extend_gap_score = -1.5

output_file = "orthologs_score.txt"

# ヒトと他種を比べて、遺伝子毎に相同性スコアの平均を算出する 
fasta_cnt = 0
for i in os.listdir(path):
    index = i.find('_')

    seq_cnt = -1
    score_list = []
    for record in SeqIO.parse(path + i, "fasta"):
        id = record.id
        desc = record.description
        seq = record.seq

        seq_cnt += 1

        if human_index[fasta_cnt] == seq_cnt: continue

        score = aligner.score(human_seq[fasta_cnt], seq)

        score_list.append(score)

    fasta_cnt += 1
    
    with open(output_file, mode='a') as output:
        output.write(str(i[:index]) + " " + str(np.mean(score_list)) + "\n")