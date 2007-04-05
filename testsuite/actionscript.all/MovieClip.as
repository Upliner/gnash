// 
//   Copyright (C) 2005, 2006 Free Software Foundation, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License

// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

//

// Test case for Function ActionScript class
// compile this test case with Ming makeswf, and then
// execute it like this gnash -1 -r 0 -v out.swf

rcsid="$Id: MovieClip.as,v 1.41 2007/04/05 15:03:50 strk Exp $";

#include "check.as"

#if OUTPUT_VERSION < 6
note("WARNING: it has been reported that adobe flash player version 9 fails a few tests here.");
note("         We belive those are actually adobe player bugs since older versions ");
note("         of the player are reported to pass all tests. If you have another idea ");
note("         we'd be glad to hear from you, just check the testcase source code.");
note();
#endif

// Get a reference to a MovieClip
var mc = _root;
check(typeof(mc)=="movieclip");

// Check some references
check_equals(typeof(this), 'movieclip');
check_equals(typeof(_parent), 'undefined');
check_equals(_root, this);

// Check inheritance
check(MovieClip);
check_equals(mc.__proto__, MovieClip.prototype);
check_equals(typeof(MovieClip.prototype._width), "undefined");
check_equals(typeof(MovieClip.prototype.attachMovie), "function");
check_equals(typeof(mc._width), "number");

// Check methods existance

// SWF5 or higher
check(mc.attachMovie);
check(mc.getBytesLoaded);
check(mc.getBytesTotal);
check(mc.getBounds);
check(mc.globalToLocal);
check_equals(typeOf(mc.unloadMovie), 'function');


//
// Test getBounds (simple test)
//
var bounds = mc.getBounds();
check_equals(typeof(bounds), "object");
// Checking for real values would be a bit hard
// atm, as the loaded Dejagnu.swf file might
// write stuff all around thus making bounds
// change often... we'll check it later, with
// a user defined movieclip (more control over
// it's bounds)
check(bounds.xMin != undefined);
check(bounds.yMin != undefined);
check(bounds.xMax != undefined);
check(bounds.yMax != undefined);

// This seems unavailable
// when targetting SWF > 6
#if OUTPUT_VERSION > 6
check_equals(mc.duplicateMovieClip, undefined);
#else
check(mc.duplicateMovieClip);
#endif

#if OUTPUT_VERSION >= 6
	check(mc.beginFill);
	check(mc.beginGradientFill);
        check(mc.clear);
	check(mc.createEmptyMovieClip);
	check(mc.createTextField);
	check(mc.curveTo);
	check(mc.lineStyle);
	check(mc.lineTo);
	check(mc.moveTo);
	check(mc.attachAudio);
	check(mc.endFill);
	check(mc.getDepth);
	check(mc.getURL);
	check(mc.gotoAndPlay);
	check(mc.gotoAndStop);
	check(mc.hitTest);
	check(mc.nextFrame != undefined);
	check(mc.play != undefined);
	check(mc.prevFrame != undefined);
	check(mc.stop != undefined);
	check(mc.swapDepths != undefined);

	// These two seem unavailable
	// when targetting SWF > 6
#if OUTPUT_VERSION > 6
	check_equals(mc.loadMovie, undefined);
	check_equals(mc.removeMovieClip, undefined);
#else
	check(mc.loadMovie);
	check(mc.removeMovieClip);
#endif

#endif // OUTPUT_VERSION >= 6

#if OUTPUT_VERSION >= 7
    xcheck(mc.getInstanceAtDepth != undefined);
    xcheck(mc.getSWFVersion != undefined);
    xcheck(mc.getTextSnapshot != undefined);

    // can't confirm this works !
    // maybe we should just NOT use the _root for this ?
    //check(mc.loadVariables != undefined);

    xcheck(mc.localToGlobal);
    xcheck(mc.setMask);
    check(mc.startDrag);
    check(mc.stopDrag);
    xcheck(mc.enabled);

    // maybe this is the start condition...
    check_equals(mc.focusEnabled, undefined);
    check_equals(mc.hitArea, undefined);
    check_equals(mc.menu, undefined);

    xcheck_equals(mc.getNextHighestDepth(), 0);
#else
    check_equals(mc.getNextHighestDepth(), undefined);
#endif

// Even handlers are initially undefined, user can
// assign them a function to be called on that event...
check_equals(mc.onData, undefined);
check_equals(mc.onDragOut, undefined);
check_equals(mc.onDragOver, undefined);
check_equals(mc.onEnterFrame, undefined);
check_equals(mc.onKeyDown, undefined);
check_equals(mc.onKeyUp, undefined);
check_equals(mc.onKillFocus, undefined);
check_equals(mc.onLoad, undefined);
check_equals(mc.onMouseDown, undefined);
check_equals(mc.onMouseMove, undefined);
check_equals(mc.onMouseUp, undefined);
check_equals(mc.onPress, undefined);
check_equals(mc.onRelease, undefined);
check_equals(mc.onReleaseOutside, undefined);
check_equals(mc.onRollOut, undefined);
check_equals(mc.onRollOver, undefined);
check_equals(mc.onSetFocus, undefined);
check_equals(mc.onUnload, undefined);

// Check property existance

// These are undefined by default
check_equals(mc.tabChildren, undefined);
mc.tabChildren = false;
check_equals(mc.tabChildren, false);
mc.tabChildren = true;
check_equals(mc.tabChildren, true);
check_equals(mc.tabEnabled, undefined);
check_equals(mc.tabIndex, undefined);
check_equals(mc.trackAsMenu, undefined);
xcheck_equals(mc.useHandCursor, true);
mc.useHandCursor = false;
check_equals(mc.useHandCursor, false);
check_equals(mc._alpha, 100);
check(mc._currentframe != undefined);

#if OUTPUT_VERSION > 5
check_equals(mc._droptarget, "");
check_equals(typeof(mc._droptarget), "string");
#else
check_equals(mc._droptarget, undefined);
#endif

check(mc._focusrect != undefined);
check(mc._framesloaded != undefined);
check(mc._height != undefined);
check(mc._highquality != undefined);
check(mc._y != undefined);
check(mc._ymouse != undefined);
check(mc._yscale != undefined);
xcheck(mc._lockroot != undefined);

#if OUTPUT_VERSION > 5
check_equals(mc._name, "");
check_equals(typeof(mc._name), "string");
mc._name = "changed";
check_equals(typeof(mc._name), "string");
check_equals(typeof(mc), "movieclip");
#else
check_equals(mc._name, undefined);
#endif

check(mc._parent == undefined);
check(mc._rotation != undefined);
check(mc._soundbuftime != undefined);
check(mc._target != undefined);
check(mc._totalframes != undefined);
check(mc._url != undefined);
check(mc._visible != undefined);
check(mc._width != undefined);
check(mc._x != undefined);
check(mc._xmouse != undefined);
check(mc._xscale != undefined);

#if OUTPUT_VERSION >= 6
// Test movieclip creation
var mc2 = createEmptyMovieClip("mc2_mc", 50, 0, 0, 0);
check(mc2 != undefined);
check_equals(mc2_mc.getBytesLoaded(), 0);
check_equals(mc2_mc.getBytesTotal(), 0);
check_equals(mc2.getBytesLoaded(), 0);
check_equals(mc2.getBytesTotal(), 0);

var mc3 = createEmptyMovieClip("mc3_mc", 50);
check(mc3 != undefined);

// By default useHandCursor is false in SWF5 and true in later versions
#if OUTPUT_VERSION < 6
check_equals(mc3.useHandCursor, false);
#else
xcheck_equals(mc3.useHandCursor, true);
#endif
// We add a mouse event handler, and expect this
// to make useHandCursor true
mc3.onMouseOver = function() { trace("over"); };
xcheck_equals(mc3.useHandCursor, true);
mc3.useHandCursor = false;
check_equals(mc3.useHandCursor, false);

check_equals(mc3_mc.getBytesLoaded(), 0);
check_equals(mc3_mc.getBytesTotal(), 0);
check_equals(mc3.getBytesLoaded(), 0);
check_equals(mc3.getBytesTotal(), 0);
check_equals(mc3_mc, _level0.mc3_mc);
check_equals(String(mc3_mc), "_level0.mc3_mc");
#endif


// Test the _target property
check_equals(_root._target, "/");

#if OUTPUT_VERSION >= 6
// unfortunately we can't use createEmptyMovieClip with
// lower SWF targets...
var mc4 = _root.createEmptyMovieClip("mc4_mc", 60);
check_equals(mc4._parent, _root);
check_equals(mc4._target, "/mc4_mc");
var mc5 = mc4.createEmptyMovieClip("mc5_mc", 60);
check_equals(mc5._target, "/mc4_mc/mc5_mc");
check_equals(mc5._parent, mc4);
#endif

//----------------------------------------------
// Test timeline variables
//----------------------------------------------

var c = 1;
b = 1;
check_equals(c, 1);
check_equals(_root.c, 1);
check_equals(b, 1);
check_equals(_root.b, 1);

//----------------------------------------------
// Test new MovieClip
//----------------------------------------------

var cl = new MovieClip();
check_equals(cl.__proto__.constructor, MovieClip);
check(cl instanceOf MovieClip);
check(cl instanceOf Object);
check_equals(typeof(cl), "object");
check_equals(typeof(cl.attachMovie), "function");
check_equals(typeof(cl._width), "undefined");
check_equals(typeof(cl._parent), "undefined");

//------------------------------------------------
// Test onLoad to be allowed to be set to anything
//------------------------------------------------

_root.onLoad = 3;
check_equals(typeof(_root.onLoad), 'number');
_root.onLoad = "test";
check_equals(typeof(_root.onLoad), 'string');

//-----------------------------------------------------------
// Test $version
//-----------------------------------------------------------

#if OUTPUT_VERSION >= 6
check(this.hasOwnProperty("$version"));
#endif
check_equals(typeof(this.$version), 'string');

function enumerate(obj, enum)
{
	var enumlen = 0;
	for (var i in obj) {
		enum[i] = obj[i];
		++enumlen;
	}
	return enumlen;
}

// Check that $version is enumerable
enum = new Object; enumlen = enumerate(this, enum);
check_equals(typeof(enum['$version']), 'string');

// Check that $version is overridable and deletable
this.$version = "fake version";
check_equals(this.$version, 'fake version');
check(delete $version);
check_equals(typeof(this.$version), 'undefined');

//------------------------------------------------
// Test getProperty 
//------------------------------------------------

#ifdef MING_SUPPORTS_ASM_GETPROPERTY

asm {
	push "a"
	push "" // this doesn't resolve to top of with stack
	push 13 // _name
	getproperty
	setvariable
};
#if OUTPUT_VERSION > 5
check_equals(a, "changed");
#else
check_equals(a, undefined);
#endif

asm {
	push "b"
	push "" // this doesn't resolve to top of with stack
	push 11 // _target
	getproperty
	setvariable
};
check_equals(b, "/");

asm {
	push "a"
	push "_root"
	push 13 // _name
	getproperty
	setvariable
};
#if OUTPUT_VERSION > 5
check_equals(a, "changed");
#else
check_equals(a, undefined);
#endif

asm {
	push "b"
	push "_root"
	push 11 // _target
	getproperty
	setvariable
};
check_equals(b, "/");

#endif // MING_SUPPORT_ASM_GETPROPERTY

//------------------------------------------------
// Test createTextField
//------------------------------------------------

t = createTextField("textfieldTest", 3, 0, 100, 100, 100);
check_equals(typeof(t), 'undefined');
#if OUTPUT_VERSION > 5
check_equals(typeof(textfieldTest), 'object');
check(textfieldTest instanceof TextField);
#else
xcheck_equals(typeof(textfieldTest), 'movieclip');
xcheck(textfieldTest instanceof MovieClip);
#endif
