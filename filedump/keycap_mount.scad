

/**
* based on http://deskthority.net/w/images/c/c8/Cherry_MX_1%C3%971_keycap_specification_%281983%29.pdf
* TODO: find better source, and/or measure several brands.
*
* v.0
* 2014-May-26
*/

baseplate();
mx_mount();

/** a simple plane where to develop the keycap on top of
* NOTE: the depth of 0.95mm... it shouldn't matter because you should build
* your keycap on top of it. If not expanding the geometry with more plastic
* there, but instead using as a base for a wooden keycap cup or some
* other crazy idea, use the minimum your material/printer
* will be comfortable with
*/
module baseplate(){
	translate( [0,0, -0.9/2] )
		cube( size=[12.2,12.2,0.9], center=true );
}

/**
* The female part of the steam.
* NOTE: i'm not 100% sure on the height, as the drawing I found does not have
* it measured... and i can't infer it with full confidence when
* just accounting for the other measurements. the drawing os probably more
* concerned with the keycap per se.
*/
module mx_mount(){
	mx_height = 5.1;
	translate( [0,0,mx_height/2] ){
		difference(){
			cylinder( h=mx_height, r=5.6/2, center=true, $fn=360 );
				cube( size=[1.17,4.1,mx_height+2], center=true );
				cube( size=[4.1,1.17,mx_height+2], center=true );
		}
	}
}
