#ifndef __M_FOG_OF_WAR_H__
#define __M_FOG_OF_WAR_H__

#include "Module.h"
#include "Fog_Quadtree.h"

class Circle;
class AlphaCell;

#define ALPHA_LIMIT 255			/* Values between 0 & 255 */
#define MID_ALPHA 150			/* Values between 0 & 255 */
#define	DIVISIONS_PER_PIXELS 60	/* Values between 20 & 60 in debug for >60 fps */ /* Values between 20 & 130 in release for >60 fps */
#define RENDER_MARGIN 80		/* Values between 0 & INF */

enum FOG_TYPE
{
	NO_FOG = 0,
	GRAY_FOG,
	DARK_FOG
};

class M_FogOfWar : public Module
{
public:

	M_FogOfWar(bool startEnabled = true);
	~M_FogOfWar();

public:

	// Called each loop iteration
	update_status PostUpdate(float dt);

private:

	FOG_TYPE*				fog_layer = nullptr;	/*Layer that contains fog types*/
	AlphaCell*				alpha_layer = nullptr;	/*Layer that contains fog alpha values*/
	QuadTree<AlphaCell*>	fog_quadtree;			/*Contains the same data as the alpha layer but organized in a quadtree*/

	uint alpha_layer_width = 0;		/*Number of cells in the fog width*/
	uint alpha_layer_height = 0;	/*Number of cells in the fog height*/
	uint alpha_cell_size = 0;		/*Size of the AlphaCells wall size in pixels*/

public:

	//Generate the fog of war after the map is loaded in scene
	void		GenerateFogOfWar();

	//Get the FOG_TYPE of a concrete tile
	FOG_TYPE	GetFogID(int x, int y)const;

	//Set the alpha value x in a concrete zone (graphic layer)
	void		ClearAlphaLayer(const Circle zone, unsigned short alpha = 0, bool radiancy = false);
	//Set the FOG_TYPE x in a concrete zone (logic_layer)
	void		ClearFogLayer(const Circle zone, FOG_TYPE type);

	void DrawDebug()override;
};

#endif
