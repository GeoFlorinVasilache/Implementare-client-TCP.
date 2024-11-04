Ca sa fac tema mai intai am avut nevoie de 3 socketi principali in server si anume unul de stdin unul pentru clinetii tcp de ascultare adica cel pe care
vor venii conexiunile tcp si unul de udp , pentru clientii de udp . Pentru clinetii de tcp am considerat ca este nevoie de cate
un socket pentru fiecare client dar pentru udp nu pentru ca nu trebuia sa le dau nimic inapoi deci nu trebuia sa creez un canal de conexiune cu ei.Oricum prin udp nu se creeaza
conexiune client server. Am avut nevoie de o struct pollfd pentru , ujn vector de acest tip alocat si realocat dinamic .Iar in cazul in care in server primeam ceva pe socketul de listen
bagam un socket nou in vectorul de pollfd daca venea pe clientul udp trimiteam catre toti acei clienti tcp care erau subscriberi la topicul respectiv , si salvam intr-o lista
daca erau clientui offline care aveau acel sf pe 1 si cat ei erau offlione se primea ceva de la udp.
Cand se conecta prima data un subscriber tcp acest ii trimitea serverului informatiile despre el. Am avut nevoie de 2 liste una pentru subscriberi opnline practic si una pentru cei offline.
Cand un client se dezabona sau se inchidea acesta anunta serrverul si acesta stia sa scoata din lista de topicuri ale acelui client sau sa il scoata din vectorul de poll-uri si din lista de clienti opnlineacesta trecea astfel in lista de clienti 
offline.Iar de la tastatura serverul putea primii doar comanda exit caz in care inchidea toti cl;ientii tcp si pe el insasi. Comunicarea intre server si client
se face prim mesaje spoecifice , adica diferite cuvinte de inceput trimiose la inceputul fiecarui mesaj , cum ar fii: Inchis,UDP,etc, specific fiecarui caz. 
ca sa trimit mesaj am folosit functia de send_all care mai intai trimitea lungimea mesajului care urma si apoi mesajul propriu-zis. Recv_all care facea
practic acelasi lucru doar ca primea mesaje de o anumita lungimea , pe care o primea inaintea mesajului.