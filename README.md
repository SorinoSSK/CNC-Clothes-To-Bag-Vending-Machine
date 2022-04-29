![image](https://github.com/SorinoSSK/CNC-Clothes-To-Bag-Vending-Machine/blob/main/Resources/image%2093%20(2)%201.png)
# Stitch: The Automated Upcycling Vending-Machine
<em> A project by Brian Chin Ze Kang, Liu Tonghan, Ernest Ng Wei Jun, Seow Sin Kiat and Teo Wei Qing </em>
![image](https://github.com/SorinoSSK/CNC-Clothes-To-Bag-Vending-Machine/blob/main/Resources/image%2096.png)
<p> Given the prevalence of the issue of textile wastes (as a result consumerism) and the decline in textile recycling recycling efforts, we seeked to tackle the issue through the invention of Stitch - the very first upcycling machine that is able to convert used textiles into resuable bags. Stitch comprises of 3 main components, namely the template and the heating component and the cutting component. Both components are supplemented by other components such as the Graphic User Interface (GUI) and circuitry.</p>

![image](https://github.com/SorinoSSK/CNC-Clothes-To-Bag-Vending-Machine/blob/main/Resources/image%2093%20(3).png)
<h1> Components </h1>
<h2> Template </h2>
![image](https://github.com/SorinoSSK/CNC-Clothes-To-Bag-Vending-Machine/blob/main/Resources/T1.png)
<p> Being the component which users interact with the most, the materials of the template have been carefully selected and the template has been carefully designed in order to ensure safety and usability. Users are required to open the top template, place their T-shirts on the bottom template and and close the template in order for the whole process to begin. </p>
<p> Function(s): </p> 
<ul>
  <li> Serves as a mean to compress the T-shirt and provide stability in order to faciliate in the cutting and heating mechanisms</li>
  <li> Coloured guidelines are provided on the (bottom) template to assist users in placing their used textiles in the correct manner.</li>
</ul>
<h2> Heating Component </h2>

![image](https://github.com/SorinoSSK/CNC-Clothes-To-Bag-Vending-Machine/blob/main/Resources/image%2090.png)
<p align = "center"><em> A. Heat Stamp   B. T-shirt and Iron-on adhesive tape (inside T-shirt)   C. Bottom Template</em></p>
<p> The heating component consists of a heat stamp which is heated up when users first press on the interface and initialize the entire process. The heat stamp navigates across the end of the T-shirt with the help of a CoreXY gantry, that is able to move in the x and y coordinate plane.</p>
<p> Function(s): </p> 
<ul>
  <li> The heat stamp presses on the T-shirt and heats up the iron-on adhesive tape. The melting of the tape seals the bottom part of the T-shirt.</li>
</ul>
<h2> Cutting Component </h2>

![image](https://github.com/SorinoSSK/CNC-Clothes-To-Bag-Vending-Machine/blob/main/Resources/image%2088.png)

<p align = "center"><em> A. Cutting Blade   B. T-shirt   C. Cutting Mat</em></p>
<p> The cutting component consists of a circular cutting blade that is similarly navigated around the sleeves region of T-shirt with the assistance of the CoreXY gantry.
<p> Function(s): </p> 
<ul>
  <li> The cutting blade comes in contact with the cutting mat (which is a part of the bottom template) and is moved in a specific direction, cutting the sleeves of the T-shirt in the process.</li>
</ul>
A Fusion360 demonstration on the cutting mechanism. Note: Dimensions of individual components used in the prototype differ from this demonstration.
<p align = "center">
  <img src="https://github.com/SorinoSSK/CNC-Clothes-To-Bag-Vending-Machine/blob/main/Resources/CuttingGif.gif")
</p>
<h1> Complementary Components </h1>
<h2> Graphic User Interface (GUI) </h2>
<p> To allow new users to understand the workings of the machine and to enhance user experience, users will interact the GUI, which will be displayed on a touch-screen monitor. </p>
<p> Function(s): </p> 
<ul>
  <li> Provides clear and concise visual instructions to guide users on the know-hows of using the machine (e.g. how to place their clothes properly).</li>
  <li> Provides progress updates to let users know the status of the upcycling process.</li>
  <li> Provides safety warnings (e.g. Please close the door, Stay Clear) to promote safe use of the machine.</li>
</ul>
<h2> Circuitry </h2>

![image](https://github.com/SorinoSSK/CNC-Clothes-To-Bag-Vending-Machine/blob/main/Resources/image%2097.png)

<p> Running of the motors is essential for processes such as, but not limited to, the cutting mechanism and movement of the components and they are driven by complex circuitry that is linked to an Arduino. The monitor screen, which displays the GUI, is driven by a Raspberry Pi, that also sends a signal to the Arduino upon user's interactions with it. </p>

<h1> Rendered Machine Images</h1>

![image](https://github.com/SorinoSSK/CNC-Clothes-To-Bag-Vending-Machine/blob/main/Resources/Combined.png)

<h1> Promotional Video </h1>
<p> Click <a href ="https://vimeo.com/704222319"> here</a> to access our promotional video that showcases the inspiration behind our project and how it works! </p>
