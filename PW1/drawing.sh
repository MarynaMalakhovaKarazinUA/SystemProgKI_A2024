#!/bin/bash

colors=('\033[0;31m' '\033[0;32m' '\033[0;33m' '\033[0;34m' '\033[0;35m' '\033[0;36m')
NC='\033[0m'

echo -e "${colors[0]}      *${NC}"
echo -e "${colors[1]}     ***${NC}"
echo -e "${colors[2]}    *****${NC}"
echo -e "${colors[3]}   *******${NC}"
echo -e "${colors[4]}  *********${NC}"
echo -e "${colors[5]} ***********${NC}"
echo "*************"
echo -e "${colors[5]} ***********${NC}"
echo -e "${colors[4]}  *********${NC}"
echo -e "${colors[3]}   *******${NC}"
echo -e "${colors[2]}    *****${NC}"
echo -e "${colors[1]}     ***${NC}"
echo -e "${colors[0]}      *${NC}"

colors=('\033[1;31m' '\033[1;32m' '\033[1;33m' '\033[1;34m' '\033[1;35m' '\033[1;36m' '\033[1;37m')

for i in {1..7}; do
  echo -e "${colors[i-1]}-------------${NC}"
done

for ((count=0; count<20; count++)); do
  for i in / - \\ \|; do
    printf "\r$i"
    sleep 0.1
  done
done

for ((i=5; i>=0; i--)); do
  clear
  echo "Зворотний відлік: $i"
  sleep 1
done

for i in {1..500}; do
  sleep 0.001
  printf "\r[%-${i}s]" "$(printf "%${i}s" | tr ' ' '#')"
done

cols=$(tput cols)
rows=$(tput lines)
end=$((SECONDS+10))
while [ $SECONDS -lt $end ]; do
  clear
  for ((i=0; i<100; i++)); do
    x=$((RANDOM % cols))
    y=$((RANDOM % rows))
    printf "\033[%d;%dH*\033[0m" "$y" "$x"
  done
  sleep 0.0000001
done
clear
