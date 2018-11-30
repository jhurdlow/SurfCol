# Lightwave Modeler 2018 Surf_SetColor bug demo
Lightwave Modeler 2018.0.7 does not properly execute the Surf_SetColor command.
When called it should set the basic color of the surface, but it does not.
This code demonstrates that, as well as strange behavior when it is called
again after the user has manually set the surface color in the Surface panel.

## Running
* Build the solution in Visual Studio 2017 (Debug, x64)
* Run Modeler 2018.
* Add the plugin (SurfColBug.p)
* Run the plugin (it'll appear in the Additional menu as 'SurfColsBug')
* You should see three polygons appear.
* Open the surface panel. There should be three surfaces. They _should_ have the color associated with their name. They don't though.
* Manually set the appropriate color for each surface
* Use the translate tool to move the existing polys out of the way.
* Run the plugin again.
* Note that the surface colors change (the Red one turns blue). This should not be happening.
* Quit Modeler
* Uncomment out the conditional in the middle of the sample code (in SurfColBug.cpp)
* Build and run again.
* Repeat steps above. Note how if we don't call Surf_SetColor again, we don't get the color changes.

## TLDR;
* Surf_SetColor isn't doing anything when called for a new surface
* Surf_SetColor is wrongly changing colors on existing surfaces.