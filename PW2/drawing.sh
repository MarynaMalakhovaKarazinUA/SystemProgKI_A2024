#!/bin/bash
echo "    *"
echo "   ***"
echo "  *****"
echo " *******"
echo "*********"

RED='\033[0;31m'
NC='\033[0m'

for i in {1..10}; do
  echo -e "${RED}---------${NC}"
done

for ((count=0; count<20; count++)); do
  for i in / - \\ \|; do
    printf "\r$i"
    sleep 0.1
  done
done

for ((i=10; i>=-5; i--)); do
  clear
  echo "$i"
  sleep 1
done

for i in {1..100}; do
  sleep 0.2
  printf "\r%-${i}s" "$(printf "%${i}s" | tr ' ' '#')"
done
echo

text="Зачекайте будь ласка!"
for ((count=0; count<5; count++)); do
  for i in {1..50}; do
    printf "\r%${i}s%s" "" "$text"
    sleep 0.1
  done
done

text="Ще чекай!"
colors=(31 32 33 34 35 36 37)
for ((count=0; count<10; count++)); do
  for color in "${colors[@]}"; do
    printf "\n\r\033[0;${color}m%s\033[0m\r" "$text"
    sleep 0.15
  done
done
