<b>SPRITEY: The Sprite Sheet Animation and Frame Data Editor</b><br><br>
============================================================
<b>About Spritey</b><br><br>


Spritey is a Windows tool designed to make working with sprites (a.k.a. "Texture Atlasing" in some circles) incredibly easy  and stress-free.<br>
Spritey allows the user to import a pre-made Sprite Sheet (currently only supports .png files), specify the location of each frame of animation, and finally add collision boxes to each frame.<p>
When you're done creating your animation (hitboxes and all), save it. The animation then exists as a <b>.spr</b> file which can be loaded and displayed in your game using your renderer of choosing, containing all of the info you specified.<p>
All this wrapped-up in a simple GUI that's easy-to-use.<br>

==========================================================
<b>System Requirements</b><br><br>

You'll need the .NET framework 4.0 or later installed.<br>
Also OpenGL ver 2.1 or later. <br>

=========================================================
<b>Compiling</b><br><br>

To compile Spritey you can use the Visual Studio solution file (created using VS2010) included in the project.<br>
<b>OR</b><br>
If you don't have access to visual studio or just prefer not to use it, feel free to manually add them to another project in whatever IDE you use.
<br>

=========================================================
<b>A Few Notes</b><br><br>

-> The release folder contains everything necessary to just run the executable (if you just wish to see the program in action). Obviously aside from the entire .NET framework. You'll need to find that yourself.<br><br>
-> Although Spritey is a lovely program that I'm very proud of, I cannot claim 100% credit for every component of it. <br>
Namely, the .png decoder used to  lode the pixel data from a file into openGL. This is called LodePNG and was created by Lode Vandevenne.<br>
His site is <a href="http://lodev.org/">here</a>! (His lodepng namespace was really well documented and easy to decipher. I'd definitiely recommend it.)<br><br>
->Proper documentation for Spritey is coming soon. In particular, the SpriteyData namespace.<br>
This is important as this is the namespace that will allow users to load their .spr files into their projects and contains the structs that allow for accessing the data held within. <br>
->In the mean time, i may produce a quick'n'dirty Youtube video that quickly demonstrates Spritey and how to use her.<br> 

=========================================================
If you'd like to know anything else, just contact me.<br>
email: guy_mclean@hotmail.co.uk<br>
twitter: @GuyMac111<br>
