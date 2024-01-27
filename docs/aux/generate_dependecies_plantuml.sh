#! /bin/sh

if [ "$PWD" != "$HOME/development/tenacitas.lib" ]; then
    echo "must run from  $HOME/development/tenacitas.lib";
    exit 1
fi
u="/var/tmp/tenacitas.plantuml"; 
rm -f "$u";  
find . \( -name "*.h" -o -name "*.cpp" \)| while read h; do 
    h0=`echo $h | cut -d '/' -f 2`; 
    cat "$h" | grep "include <tenacitas" | while read i; do 
	i0=`echo "$i" | cut -d '/' -f 2`;
	echo "h = $h, h0 = $h0, i = $i0";
	echo "$h0 ..> $i0" >> "$u"; 
    done; 
done; 
mkdir ../docs 2> /dev/null; 
sort -u "$u" > /var/tmp/aux; 
u0="docs/tenacitas.lib.plantuml"; 
echo "@startuml" > "$u0"; 
echo "hide empty members" >> "$u0"; 
cat /var/tmp/aux >> "$u0"; 
echo "@enduml" >> "$u0"
