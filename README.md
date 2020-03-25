# c-bio
Bioinformatics tools written in C
## fa.c
  Returns a summary of a FastA file in JSON format.
  ```
  {
    "records": [
      {"id": "CF00165-00458.MAB", "-": 30245, "A": 908732, "C": 1627302, "G": 1617615, "T": 906597, "basepairs": 5090491},
      {"id": "CF00165-00690.MAB", "-": 28640, "A": 909056, "C": 1627816, "G": 1618104, "T": 906875, "basepairs": 5090491}, 
      {"id": "CF00167-00168.MAB", "-": 421518, "A": 830975, "C": 1507804, "G": 1498975, "T": 831219, "basepairs": 5090491},
      {"id": "CF00167-00216.MAB", "-": 426886, "A": 830196, "C": 1505971, "G": 1497011, "T": 830427, "basepairs": 5090491},
    ]
  }
  ```
## matrix-metrics.cc
  Returns 
