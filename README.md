Game-Programming-WS2014
=======================
**Idee**

Inspiriert wurden wir von Mammut und Krautscape. Du musst dich bewegen. Du kannst nicht anhalten. Du kennst dein Ziel. Der Weg... existiert noch nicht. Während du dich fortbewegst, wird die Strecke unter deinem Einfluss generiert!

=======================
**Konzept (Anreize, Schwierigkeitsgrad, geforderte Skills...)**

Levelbasiertes Spiel. Ein Level besteht aus Start und Ziel, einem Farbschema und Schwierigkeitsgrad, der sich in verschiedenen Aspekten äußert. Jedes Level soll automatisch generiert werden aus einer Auswahl von Farbschemata und Schwierigkeitsgraden. Der Schwierigkeitsgrad soll von Level zu Level zufällig ansteigen. Wenn der Spieler das Ziel erreicht, wird seine Bewegung pausiert und es kommt zum Levelübergang. Es wird so die Kamera bewegt (gezoomt), dass die gesamte Strecke und der Start zu sehen ist; der Start bleibt erhalten, der Rest der Welt (Ziel und generierte Strecke) wird verzerrt und in den Start gezogen; ist alles verschwunden, erscheint die Spielfigur im Start und das neue Ziel.

Schwierigkeitsgrad:
* Geschwindigkeit der Spielfigur
* Anzahl Hindernisse
* Anzahl Löcher in der Bahn
* Form der Bahn (glatt, halbe Pipeline, Tunnel) 
* Absturzmöglichkeiten am Rand der Bahn 
* Anzahl Items (negative, neutrale, positive)

Die Geschicklichkeit und die Reaktionsfähigkeit des Spielers sollen herausgefordert werden.

=======================
**Details**

Spielfigur
Ein Torus, eventuell mit verschiedenen Skins. 

Steuerung
Die Spielfigur soll am Desktop über Pfeiltasten, am mobilen Gerät über Touch gesteuert werden. Die Streckengenerierung soll am Desktop über die Maus, am mobilen Gerät über den Neigungswinkel gesteuert werden. Die Kamera soll vom Spieler nicht steuerbar sein, sondern ist auf die Streckenmitte zentriert.

Streckengenerierung
Der Spieler soll über die Streckengenerierung das Ziel ansteuern. Das heißt, der Spieler beeinflusst, ob die Strecke eine Links-/Rechtskurve (Steigung/Senkung optional) generiert wird. Das Spiel "entscheidet", wie die Strecke aussieht. Vorstellbar ist alles von einer flache Strecke, über eine halbe Pipeline bis zum Tunnel. Des Weiteren könnte es Hindernisse und Boni jeglicher Art geben.

=======================
**Grafik**

Motion-Blur

=======================
**Stil**

Geometrische Landschaft, abstrakte Formen. Dunkler Hintergrund. Schwarze Gegenstände, neonfarbene Outlines. Glow-Effekt. 

Ziel als Ring, Portal mit coolen Effekten. 
Start auf einem Podest.

Der nächste, von der Mausposition abhängige, zu generierende Streckenabschnitt wird angedeutet.

Die Spielfigur glüht und zieht eine Spur hinter sich her.

=======================
**Sound**

Es soll ein Item geben, bei dem abhängig von der Musik Blendeffekte im Raum erzeugt werden(, um den Nutzer zu überfordern - negatives Item). 

Musik soll vom Nutzer in das Spiel geladen werden können.   
