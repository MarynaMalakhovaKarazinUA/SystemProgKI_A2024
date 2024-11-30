#!/bin/bash
echo "    *"
echo "   ***"
echo "  *****"
echo " *******"
echo "*********"

RED='\033[0;31m'
NC='\033[0m'

for i in {1..10}; do
  echo -e "${RED}-----${NC}"
done

for ((count=0; count<20; count++)); do
  for i in / - \\ \|; do
    printf "\r$i"
    sleep 0.1
  done
done

end=$((SECONDS+10))
while [ $SECONDS -lt $end ]; do
  clear
  date +"%T"
  sleep 1
done

for i in {1..100}; do
  sleep 0.1
  printf "\r[%-${i}s]" "$(printf "%${i}s" | tr ' ' '#')"
done
echo

text="Танцюючий текст!"
for ((count=0; count<20; count++)); do
  for i in {1..40}; do
    printf "\r%${i}s%s" "" "$text"
    sleep 0.1
  done
done

text="Радіо-передача!"
colors=(31 32 33 34 35 36 37)
for ((count=0; count<10; count++)); do
  for color in "${colors[@]}"; do
    printf "\033[0;${color}m%s\033[0m\r" "$text"
    sleep 0.5
  done
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
  sleep 0.1
done
