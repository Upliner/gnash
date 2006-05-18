// text.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Code for the text tags.


#include "utf8.h"
#include "utility.h"
#include "impl.h"
//#include "shape.h"
#include "shape_character_def.h"
#include "stream.h"
#include "log.h"
#include "font.h"
#include "fontlib.h"
#include "render.h"
#include "textformat.h"
#include "text.h"
#include "movie_definition.h"

#include "edit_text_character_def.h"

namespace gnash {

void
edit_text_character_def::read(stream* in, int tag_type,
		movie_definition* m)
{
	assert(m != NULL);
	assert(tag_type == 37);

	m_rect.read(in);

	in->align();
	bool	has_text = in->read_uint(1) ? true : false;
	m_word_wrap = in->read_uint(1) ? true : false;
	m_multiline = in->read_uint(1) ? true : false;
	m_password = in->read_uint(1) ? true : false;
	m_readonly = in->read_uint(1) ? true : false;
	bool	has_color = in->read_uint(1) ? true : false;
	bool	has_max_length = in->read_uint(1) ? true : false;
	bool	has_font = in->read_uint(1) ? true : false;

	in->read_uint(1);	// reserved
	m_auto_size = in->read_uint(1) ? true : false;
	bool	has_layout = in->read_uint(1) ? true : false;
	m_no_select = in->read_uint(1) ? true : false;
	m_border = in->read_uint(1) ? true : false;
	in->read_uint(1);	// reserved
	m_html = in->read_uint(1) ? true : false;
	m_use_outlines = in->read_uint(1) ? true : false;

	if (has_font)
	{
		m_font_id = in->read_u16();
		m_text_height = (float) in->read_u16();
	}

	if (has_color)
	{
		m_color.read_rgba(in);
	}

	if (has_max_length)
	{
		m_max_length = in->read_u16();
	}

	if (has_layout)
	{
		m_alignment = (alignment) in->read_u8();
		m_left_margin = (float) in->read_u16();
		m_right_margin = (float) in->read_u16();
		m_indent = (float) in->read_s16();
		m_leading = (float) in->read_s16();
	}

	char*	name = in->read_string();
	m_default_name = name;
	delete [] name;

	if (has_text)
	{
		char*	str = in->read_string();
		m_default_text = str;
		delete [] str;
	}

	IF_VERBOSE_PARSE(log_msg("edit_text_char, varname = %s, text = %s\n",
				 m_default_name.c_str(), m_default_text.c_str()));
}



//
// edit_text_character
//

void
edit_text_character::set_text_value(const char* new_text)
{
	if (m_text == new_text)
	{
		return;
	}

	m_text = new_text;
	if (m_def->m_max_length > 0
	    && m_text.length() > m_def->m_max_length)
	{
		m_text.resize(m_def->m_max_length);
	}

	format_text();
}

void
edit_text_character::set_member(const tu_stringi& name,
		const as_value& val)
{
	// @@ TODO need to inherit basic stuff like _x, _y, _xscale, _yscale etc

	as_standard_member	std_member = get_standard_member(name);
	switch (std_member)
	{
	default:
	case M_INVALID_MEMBER:
		break;
	case M_TEXT:
		//if (name == "text")
	{
		int version = get_parent()->get_movie_definition()->get_version();
		set_text_value(val.to_tu_string_versioned(version).c_str());
		return;
	}
	case M_X:
		//else if (name == "_x")
	{
		matrix	m = get_matrix();
		m.m_[0][2] = (float) PIXELS_TO_TWIPS(val.to_number());
		set_matrix(m);

		// m_accept_anim_moves = false;
		
		return;
	}
	case M_Y:
		//else if (name == "_y")
	{
		matrix	m = get_matrix();
		m.m_[1][2] = (float) PIXELS_TO_TWIPS(val.to_number());
		set_matrix(m);

		// m_accept_anim_moves = false;
		
		return;
	}
	case M_VISIBLE:
		//else if (name == "_visible")
	{
		set_visible(val.to_bool());
		return;
	}
	case M_ALPHA:
		//else if (name == "_alpha")
	{
		// @@ TODO this should be generic to struct character!
		// Arg is in percent.
		cxform	cx = get_cxform();
		cx.m_[3][0] = fclamp(float(val.to_number()) / 100.f, 0, 1);
		set_cxform(cx);
		return;
	}
	case M_TEXTCOLOR:
		//else if (name == "textColor")
	{	
		// The arg is 0xRRGGBB format.
		uint32_t	rgb = (uint32_t) val.to_number();

		cxform	cx = get_cxform();
		cx.m_[0][0] = fclamp(((rgb >> 16) & 255) / 255.0f, 0, 1);
		cx.m_[1][0] = fclamp(((rgb >>  8) & 255) / 255.0f, 0, 1);
		cx.m_[2][0] = fclamp(((rgb      ) & 255) / 255.0f, 0, 1);
		set_cxform(cx);

		return;
	}
	// @@ TODO see TextField members in Flash MX docs
	}	// end switch
}

bool
edit_text_character::get_member(const tu_stringi& name, as_value* val)
{
	as_standard_member	std_member = get_standard_member(name);
	switch (std_member)
	{
	default:
	case M_INVALID_MEMBER:
		break;
	case M_TEXT:
		//if (name == "text")
	{
		val->set_tu_string(m_text);
		return true;
	}
	case M_VISIBLE:
		//else if (name == "_visible")
	{
		val->set_bool(get_visible());
		return true;
	}
	case M_ALPHA:
		//else if (name == "_alpha")
	{
		// @@ TODO this should be generic to struct character!
		const cxform&	cx = get_cxform();
		val->set_double(cx.m_[3][0] * 100.f);
		return true;
	}
	case M_TEXTCOLOR:
		//else if (name == "textColor")
	{
		// Return color in 0xRRGGBB format
		const cxform&	cx = get_cxform();
		int	r = iclamp(int(cx.m_[0][0] * 255), 0, 255);
		int	g = iclamp(int(cx.m_[0][0] * 255), 0, 255);
		int	b = iclamp(int(cx.m_[0][0] * 255), 0, 255);
		val->set_int((r << 16) + (g << 8) + b);
		return true;
	}
	case M_X:
		//else if (name == "_x")
	{
		matrix	m = get_matrix();	// @@ get_world_matrix()???
		val->set_double(TWIPS_TO_PIXELS(m.m_[0][2]));
		return true;
	}
	case M_Y:
		//else if (name == "_y")
	{
		matrix	m = get_matrix();	// @@ get_world_matrix()???
		val->set_double(TWIPS_TO_PIXELS(m.m_[1][2]));
		return true;
	}
	case M_WIDTH:
		//else if (name == "_width")
	{
		// @@ TODO should implement this in
		// character and inherit into both here and sprite_instance
		rect	transformed_rect;
		transformed_rect.enclose_transformed_rect(get_world_matrix(), m_def->m_rect);
		val->set_double(TWIPS_TO_PIXELS(transformed_rect.width()));
		return true;
	}
	case M_HEIGHT:
		//else if (name == "_height")
	{
		// @@ TODO should implement this in
		// character and inherit into both here and sprite_instance
		rect	transformed_rect;
		transformed_rect.enclose_transformed_rect(get_world_matrix(), m_def->m_rect);
		val->set_double(TWIPS_TO_PIXELS(transformed_rect.height()));
		return true;
	}
	case M_TEXTWIDTH:
		//else if (name == "textWidth")
	{
		// Return the width, in pixels, of the text as laid out.
		// (I.e. the actual text content, not our defined
		// bounding box.)
		//
		// In local coords.  Verified against Macromedia Flash.
		val->set_double(TWIPS_TO_PIXELS(m_text_bounding_box.width()));

		return true;
	}
	}	// end switch

	return false;
}
	
// @@ WIDTH_FUDGE is a total fudge to make it match the Flash player!  Maybe
// we have a bug?
#define WIDTH_FUDGE 80.0f


void
edit_text_character::align_line(
		edit_text_character_def::alignment align,
		int last_line_start_record, float x)
{
	assert(m_def);
	float	extra_space = (m_def->m_rect.width() -
			m_def->m_right_margin) - x - WIDTH_FUDGE;
	assert(extra_space >= 0.0f);

	float	shift_right = 0.0f;

	if (align == edit_text_character_def::ALIGN_LEFT)
	{
		// Nothing to do; already aligned left.
		return;
	}
	else if (align == edit_text_character_def::ALIGN_CENTER)
	{
		// Distribute the space evenly on both sides.
		shift_right = extra_space / 2;
	}
	else if (align == edit_text_character_def::ALIGN_RIGHT)
	{
		// Shift all the way to the right.
		shift_right = extra_space;
	}

	// Shift the beginnings of the records on this line.
	for (unsigned int i = last_line_start_record; i < m_text_glyph_records.size(); i++)
	{
		text_glyph_record&	rec = m_text_glyph_records[i];

		if (rec.m_style.m_has_x_offset)
		{
			rec.m_style.m_x_offset += shift_right;
		}
	}
}

void
edit_text_character::format_text()
{
	m_text_glyph_records.resize(0);

	if (m_def->m_font == NULL)
	{
		return;
	}

	// @@ mostly for debugging
	// Font substitution -- if the font has no
	// glyphs, try some other defined font!
	if (m_def->m_font->get_glyph_count() == 0)
	{
		// Find a better font.
		font*	newfont = m_def->m_font;
		for (int i = 0, n = fontlib::get_font_count(); i < n; i++)
		{
			font*	f = fontlib::get_font(i);
			assert(f);

			if (f->get_glyph_count() > 0)
			{
				// This one looks good.
				newfont = f;
				break;
			}
		}

		if (m_def->m_font != newfont)
		{
			log_error("error: substituting font!  font '%s' has no glyphs, using font '%s'\n",
				  fontlib::get_font_name(m_def->m_font),
				  fontlib::get_font_name(newfont));

			m_def->m_font = newfont;
		}
	}


	float	scale = m_def->m_text_height / 1024.0f;	// the EM square is 1024 x 1024

	text_glyph_record	rec;	// one to work on
	rec.m_style.m_font = m_def->m_font;
	rec.m_style.m_color = m_def->m_color;
	rec.m_style.m_x_offset = fmax(0, m_def->m_left_margin + m_def->m_indent);
	rec.m_style.m_y_offset = m_def->m_text_height
		+ (m_def->m_font->get_leading() - m_def->m_font->get_descent()) * scale;
	rec.m_style.m_text_height = m_def->m_text_height;
	rec.m_style.m_has_x_offset = true;
	rec.m_style.m_has_y_offset = true;

	float	x = rec.m_style.m_x_offset;
	float	y = rec.m_style.m_y_offset;

	// Start the bbox at the upper-left corner of the first glyph.
	reset_bounding_box(x, y - m_def->m_font->get_descent() * scale + m_def->m_text_height);

	float	leading = m_def->m_leading;
	leading += m_def->m_font->get_leading() * scale;

	int	last_code = -1;
	int	last_space_glyph = -1;
	int	last_line_start_record = 0;

	const char*	text = &m_text[0];
	while (uint32_t code = utf8::decode_next_unicode_character(&text))
	{
// @@ try to truncate overflow text??
#if 0
		if (y + m_def->m_font->get_descent() * scale > m_def->m_rect.height())
		{
			// Text goes below the bottom of our bounding box.
			rec.m_glyphs.resize(0);
			break;
		}
#endif // 0

		//uint16_t	code = m_text[j];

		x += m_def->m_font->get_kerning_adjustment(last_code, (int) code) * scale;
		last_code = (int) code;

		// Expand the bounding-box to the lower-right corner of each glyph as
		// we generate it.
		m_text_bounding_box.expand_to_point(x, y + m_def->m_font->get_descent() * scale);

		if (code == 13 || code == 10)
		{
			// newline.

			// Frigging Flash seems to use '\r' (13) as its
			// default newline character.  If we get DOS-style \r\n
			// sequences, it'll show up as double newlines, so maybe we
			// need to detect \r\n and treat it as one newline.

			// Close out this stretch of glyphs.
			m_text_glyph_records.push_back(rec);
			align_line(m_def->m_alignment, last_line_start_record, x);

			x = fmax(0, m_def->m_left_margin + m_def->m_indent);	// new paragraphs get the indent.
			y += m_def->m_text_height + leading;

			// Start a new record on the next line.
			rec.m_glyphs.resize(0);
			rec.m_style.m_font = m_def->m_font;
			rec.m_style.m_color = m_def->m_color;
			rec.m_style.m_x_offset = x;
			rec.m_style.m_y_offset = y;
			rec.m_style.m_text_height = m_def->m_text_height;
			rec.m_style.m_has_x_offset = true;
			rec.m_style.m_has_y_offset = true;

			last_space_glyph = -1;
			last_line_start_record = m_text_glyph_records.size();

			continue;
		}

		if (code == 8)
		{
			// backspace (ASCII BS).

			// This is a limited hack to enable overstrike effects.
			// It backs the cursor up by one character and then continues
			// the layout.  E.g. you can use this to display an underline
			// cursor inside a simulated text-entry box.
			//
			// ActionScript understands the '\b' escape sequence
			// for inserting a BS character.
			//
			// ONLY WORKS FOR BACKSPACING OVER ONE CHARACTER, WON'T BS
			// OVER NEWLINES, ETC.

			if (rec.m_glyphs.size() > 0)
			{
				// Peek at the previous glyph, and zero out its advance
				// value, so the next char overwrites it.
				float	advance = rec.m_glyphs.back().m_glyph_advance;
				x -= advance;	// maintain formatting
				rec.m_glyphs.back().m_glyph_advance = 0;	// do the BS effect
			}
			continue;
		}

		// Remember where word breaks occur.
		if (code == 32)
		{
			last_space_glyph = rec.m_glyphs.size();
		}

		int	index = m_def->m_font->get_glyph_index((uint16_t) code);
		if (index == -1)
		{
			// error -- missing glyph!
			
			// Log an error, but don't log too many times.
			static int	s_log_count = 0;
			if (s_log_count < 10)
			{
				s_log_count++;
				log_warning("%s\n\t -- missing glyph for char %d\n"
					    "-- make sure character shapes for font %s are being exported "
					  "into your SWF file!\n",
					    __PRETTY_FUNCTION__,
					    code,
					    m_def->m_font->get_name());
			}

			// Drop through and use index == -1; this will display
			// using the empty-box glyph
		}
		text_glyph_record::glyph_entry	ge;
		ge.m_glyph_index = index;
		ge.m_glyph_advance = scale * m_def->m_font->get_advance(index);

		rec.m_glyphs.push_back(ge);

		x += ge.m_glyph_advance;

		
		if (x >= m_def->m_rect.width() - m_def->m_right_margin - WIDTH_FUDGE)
		{
			// Whoops, we just exceeded the box width.  Do word-wrap.

			// Insert newline.

			// Close out this stretch of glyphs.
			m_text_glyph_records.push_back(rec);
			float	previous_x = x;

			x = m_def->m_left_margin;
			y += m_def->m_text_height + leading;

			// Start a new record on the next line.
			rec.m_glyphs.resize(0);
			rec.m_style.m_font = m_def->m_font;
			rec.m_style.m_color = m_def->m_color;
			rec.m_style.m_x_offset = x;
			rec.m_style.m_y_offset = y;
			rec.m_style.m_text_height = m_def->m_text_height;
			rec.m_style.m_has_x_offset = true;
			rec.m_style.m_has_y_offset = true;
			
			text_glyph_record&	last_line = m_text_glyph_records.back();
			if (last_space_glyph == -1)
			{
				// Pull the previous glyph down onto the
				// new line.
				if (last_line.m_glyphs.size() > 0)
				{
					rec.m_glyphs.push_back(last_line.m_glyphs.back());
					x += last_line.m_glyphs.back().m_glyph_advance;
					previous_x -= last_line.m_glyphs.back().m_glyph_advance;
					last_line.m_glyphs.resize(last_line.m_glyphs.size() - 1);
				}
			}
			else
			{
				// Move the previous word down onto the next line.

				previous_x -= last_line.m_glyphs[last_space_glyph].m_glyph_advance;

				for (unsigned int i = last_space_glyph + 1; i < last_line.m_glyphs.size(); i++)
				{
					rec.m_glyphs.push_back(last_line.m_glyphs[i]);
					x += last_line.m_glyphs[i].m_glyph_advance;
					previous_x -= last_line.m_glyphs[i].m_glyph_advance;
				}
				last_line.m_glyphs.resize(last_space_glyph);
			}

			align_line(m_def->m_alignment, last_line_start_record, previous_x);

			last_space_glyph = -1;
			last_line_start_record = m_text_glyph_records.size();
		}

		// TODO: HTML markup
	}

	// Add this line to our output.
	m_text_glyph_records.push_back(rec);
	align_line(m_def->m_alignment, last_line_start_record, x);
}

void
edit_text_character::display()
{
//		GNASH_REPORT_FUNCTION;

	if (m_def->m_border)
	{
		matrix	mat = get_world_matrix();
		
		// @@ hm, should we apply the color xform?  It seems logical; need to test.
		// cxform	cx = get_world_cxform();

		// Show white background + black bounding box.
		render::set_matrix(mat);

		point	coords[4];
		coords[0] = m_def->m_rect.get_corner(0);
		coords[1] = m_def->m_rect.get_corner(1);
		coords[2] = m_def->m_rect.get_corner(3);
		coords[3] = m_def->m_rect.get_corner(2);

		int16_t	icoords[18] = 
		{
			// strip (fill in)
			(int16_t) coords[0].m_x, (int16_t) coords[0].m_y,
			(int16_t) coords[1].m_x, (int16_t) coords[1].m_y,
			(int16_t) coords[2].m_x, (int16_t) coords[2].m_y,
			(int16_t) coords[3].m_x, (int16_t) coords[3].m_y,

			// outline
			(int16_t) coords[0].m_x, (int16_t) coords[0].m_y,
			(int16_t) coords[1].m_x, (int16_t) coords[1].m_y,
			(int16_t) coords[3].m_x, (int16_t) coords[3].m_y,
			(int16_t) coords[2].m_x, (int16_t) coords[2].m_y,
			(int16_t) coords[0].m_x, (int16_t) coords[0].m_y,
		};
		
		render::fill_style_color(0, rgba(255, 255, 255, 255));
		render::draw_mesh_strip(&icoords[0], 4);

		render::line_style_color(rgba(0,0,0,255));
		render::draw_line_strip(&icoords[8], 5);
	}

	// Draw our actual text.
	display_glyph_records(matrix::identity, this, m_text_glyph_records, m_def->m_root_def);

	do_display_callback();
}

//
// edit_text_character_def
//

character*	edit_text_character_def::create_character_instance(movie* parent, int id)
{
	if (m_font == NULL)
	{
		// Resolve the font, if possible.
		m_font = m_root_def->get_font(m_font_id);
		if (m_font == NULL)
		{
			log_error("error: text style with undefined font; font_id = %d\n", m_font_id);
		}
	}

	edit_text_character*	ch = new edit_text_character(parent, this, id);
	ch->set_name(m_default_name.c_str());
	return ch;
}


} // namespace gnash


// Local Variables:
// mode: C++
// indent-tabs-mode: t
// End:

