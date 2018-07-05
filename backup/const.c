 /***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1995 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

/***************************************************************************
*       ROT 1.4 is copyright 1996-1997 by Russ Walsh                       *
*       By using this code, you have agreed to follow the terms of the     *
*       ROT license, in the file doc/rot.license                           *
***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"
#include "recycle.h"


char *	const	dir_name	[]		=
{
    "north", "east", "south", "west", "up", "down",
    "north", "east", "south", "west", "up", "down"
};

const	sh_int	rev_dir		[]		=
{
    2, 3, 0, 1, 5, 4, 2, 3, 0, 1, 5, 4
};

const	sh_int	movement_loss	[SECT_MAX]	=
{
    1, 2, 2, 3, 4, 6, 4, 1, 6, 10, 6
};

const struct bank_type		bank_table	[]	=
{
    {	"First National Bank of Midgard",
	3334,	8,	20	},
    {	"Aerial City Credit Union",
	22044,	8,	16	},
    {	"Solace Federal Reserve",
	10267,	9,	20	},
    {	"New Thalos Savings and Trust",
	9800,	7,	19	}
};

/* item type list */
const struct item_type		item_table	[]	=
{
    {	ITEM_LIGHT,	"light"		},
    {	ITEM_SCROLL,	"scroll"	},
    {	ITEM_WAND,	"wand"		},
    {   ITEM_STAFF,	"staff"		},
    {   ITEM_WEAPON,	"weapon"	},
    {   ITEM_TREASURE,	"treasure"	},
    {   ITEM_ARMOR,	"armor"		},
    {	ITEM_POTION,	"potion"	},
    {	ITEM_CLOTHING,	"clothing"	},
    {   ITEM_FURNITURE,	"furniture"	},
    {	ITEM_TRASH,	"trash"		},
    {	ITEM_CONTAINER,	"container"	},
    {	ITEM_DRINK_CON, "drink"		},
    {	ITEM_KEY,	"key"		},
    {	ITEM_FOOD,	"food"		},
    {	ITEM_MONEY,	"money"		},
    {	ITEM_BOAT,	"boat"		},
    {	ITEM_CORPSE_NPC,"npc_corpse"	},
    {	ITEM_CORPSE_PC,	"pc_corpse"	},
    {   ITEM_FOUNTAIN,	"fountain"	},
    {	ITEM_PILL,	"pill"		},
    {	ITEM_PROTECT,	"protect"	},
    {	ITEM_MAP,	"map"		},
    {	ITEM_PORTAL,	"portal"	},
    {	ITEM_WARP_STONE,"warp_stone"	},
    {	ITEM_ROOM_KEY,	"room_key"	},
    {	ITEM_GEM,	"gem"		},
    {	ITEM_JEWELRY,	"jewelry"	},
    {   ITEM_JUKEBOX,	"jukebox"	},
    {	ITEM_DEMON_STONE,"demon_stone"	},
    {	ITEM_EXIT,	"exit"		},
    {	ITEM_PIT,	"pit"		},
    {	ITEM_PASSBOOK,	"passbook"	},
    {   ITEM_SKELETON,  "skeleton"      },
    {   ITEM_SLOT_MACHINE,"slotmachine" },
  //{   ITEM_QUEST,     "quest"         },
    {   0,		NULL		}
};


/* weapon selection table */
const	struct	weapon_type	weapon_table	[]	=
{
   { "sword",	OBJ_VNUM_SCHOOL_SWORD,	WEAPON_SWORD,	&gsn_sword	},
   { "mace",	OBJ_VNUM_SCHOOL_MACE,	WEAPON_MACE,	&gsn_mace 	},
   { "dagger",	OBJ_VNUM_SCHOOL_DAGGER,	WEAPON_DAGGER,	&gsn_dagger	},
   { "axe",	OBJ_VNUM_SCHOOL_AXE,	WEAPON_AXE,	&gsn_axe	},
   { "staff",	OBJ_VNUM_SCHOOL_STAFF,	WEAPON_SPEAR,	&gsn_spear	},
   { "flail",	OBJ_VNUM_SCHOOL_FLAIL,	WEAPON_FLAIL,	&gsn_flail	},
   { "whip",	OBJ_VNUM_SCHOOL_WHIP,	WEAPON_WHIP,	&gsn_whip	},
   { "polearm",	OBJ_VNUM_SCHOOL_POLEARM,WEAPON_POLEARM,	&gsn_polearm	},
 //{ "staff",   OBJ_VNUM_SCHOOL_STAFF, WEAPON_STAFF, &gsn_staff         },
   { NULL,	0,				0,	NULL		}
};



/* wiznet table and prototype for future flag setting */
const   struct wiznet_type      wiznet_table    []              =
{
   {    "on",           WIZ_ON,         0  },
   {    "prefix",	WIZ_PREFIX,	0  },
   {    "ticks",        WIZ_TICKS,      0  },
   {    "logins",       WIZ_LOGINS,     IM },
   {    "sites",        WIZ_SITES,      IM },
   {    "links",        WIZ_LINKS,      L7 },
   {	"newbies",	WIZ_NEWBIE,	0  },
   {	"spam",		WIZ_SPAM,	L5 },
   {    "deaths",       WIZ_DEATHS,     IM },
   {    "resets",       WIZ_RESETS,     L4 },
   {    "mobdeaths",    WIZ_MOBDEATHS,  L4 },
   {    "flags",	WIZ_FLAGS,	L5 },
   {	"penalties",	WIZ_PENALTIES,	L5 },
   {	"saccing",	WIZ_SACCING,	L5 },
   {	"levels",	WIZ_LEVELS,	0  },
   {	"load",		WIZ_LOAD,	L2 },
   {	"restore",	WIZ_RESTORE,	L2 },
   {	"snoops",	WIZ_SNOOPS,	L2 },
   {	"switches",	WIZ_SWITCHES,	L2 },
   {	"secure",	WIZ_SECURE,	L1 },
   {    "debug",        WIZ_DEBUG,      L2 },
   {	NULL,		0,		0  }
};

/* attack table  -- not very organized :(
 * Be sure to keep MAX_DAMAGE_MESSAGE up
 * to date in merc.h
 */
const 	struct attack_type	attack_table	[]		=
{
    { 	"none",		"hit",		-1		},  /*  0 */
    {	"slice",	"slice", 	DAM_SLASH	},
    {   "stab",		"stab",		DAM_PIERCE	},
    {	"slash",	"slash",	DAM_SLASH	},
    {	"whip",		"whip",		DAM_SLASH	},
    {   "claw",		"claw",		DAM_SLASH	},  /*  5 */
    {	"blast",	"blast",	DAM_BASH	},
    {   "pound",	"pound",	DAM_BASH	},
    {	"crush",	"crush",	DAM_BASH	},
    {   "grep",		"grep",		DAM_SLASH	},
    {	"bite",		"bite",		DAM_PIERCE	},  /* 10 */
    {   "pierce",	"pierce",	DAM_PIERCE	},
    {   "suction",	"suction",	DAM_BASH	},
    {	"beating",	"beating",	DAM_BASH	},
    {   "digestion",	"digestion",	DAM_ACID	},
    {	"charge",	"charge",	DAM_BASH	},  /* 15 */
    { 	"slap",		"slap",		DAM_BASH	},
    {	"punch",	"punch",	DAM_BASH	},
    {	"wrath",	"wrath",	DAM_ENERGY	},
    {	"magic",	"magic",	DAM_ENERGY	},
    {   "divine",	"divine power",	DAM_HOLY	},  /* 20 */
    {	"cleave",	"cleave",	DAM_SLASH	},
    {	"scratch",	"scratch",	DAM_PIERCE	},
    {   "peck",		"peck",		DAM_PIERCE	},
    {   "peckb",	"peck",		DAM_BASH	},
    {   "chop",		"chop",		DAM_SLASH	},  /* 25 */
    {   "sting",	"sting",	DAM_PIERCE	},
    {   "smash",	 "smash",	DAM_BASH	},
    {   "shbite",	"shocking bite",DAM_LIGHTNING	},
    {	"flbite",	"flaming bite", DAM_FIRE	},
    {	"frbite",	"freezing bite", DAM_COLD	},  /* 30 */
    {	"acbite",	"acidic bite", 	DAM_ACID	},
    {	"chomp",	"chomp",	DAM_PIERCE	},
    {  	"drain",	"life drain",	DAM_NEGATIVE	},
    {   "thrust",	"thrust",	DAM_PIERCE	},
    {   "slime",	"slime",	DAM_ACID	},
    {	"shock",	"shock",	DAM_LIGHTNING	},
    {   "thwack",	"thwack",	DAM_BASH	},
    {   "flame",	"flame",	DAM_FIRE	},
    {   "chill",	"chill",	DAM_COLD	},
    {   "typo",		"typo",		DAM_SLASH	}, /* 40 */
    {   "blaze",        "blaze",        DAM_FIRE        },
    {   "flash",        "flash",        DAM_LIGHT       },
    {   "fury",         "fury",         DAM_ENERGY      },
    {   "wave",         "wave",         DAM_DROWNING    },
    {   "psias",        "psionic assault", DAM_MENTAL   },
    {   "sonic",        "sonic blast",   DAM_SOUND      },
//    {   "mythic",       "Vengence",      DAM_ELEMENTAL  }, //problem -bree
    {   NULL,		NULL,		0		}

};

bool is_racename(char *name)
{
 int race;

 if(name[0] == '\0')
  return FALSE;

 for (race = 1; race_table[race].name != NULL; race++ )
 {
  if(!str_cmp(name,race_table[race].name))
   return TRUE;
 }
 return FALSE;
}

bool is_classname(char *name)
{
 int class;

 class = class_lookup(name);

 if(class == -1)
  return FALSE;

 return TRUE;
}


void do_racehelp(CHAR_DATA *ch, char *argument)
{
 char buf[MSL/4];
 int race,pcrace = -1,i;
 BUFFER *output;

 output = new_buf();

 race = race_lookup(argument);

 if(race_table[race].pc_race)
  pcrace = pc_race_lookup(argument);

 sprintf(buf,"{&---------- {!Race: {^%s{& ----------{x\n\r",race_table[race].name);
 add_buf(output,buf);
 sprintf(buf,"{&Player Race: {#%s{x\n\r", race_table[race].pc_race ? "True" : "False");
 add_buf(output,buf);
 sprintf(buf,"{&Act Flags: {#%s{x\n\r", act_bit_name(race_table[race].act));
 add_buf(output,buf);
 sprintf(buf,"{&Offense Flags: {#%s{x\n\r", off_bit_name(race_table[race].off));
 add_buf(output,buf);
 sprintf(buf,"{&Immune To: {#%s{x\n\r", imm_bit_name(race_table[race].imm));
 add_buf(output,buf);
 sprintf(buf,"{&Resistant To: {#%s{x\n\r", res_bit_name(race_table[race].res));
 add_buf(output,buf);
 sprintf(buf,"{&Vulnerable To: {#%s{x\n\r", vuln_bit_name(race_table[race].vuln));
 add_buf(output,buf);
 sprintf(buf,"{&Shielded By: {#%s{x\n\r", shield_bit_name(race_table[race].shd));
 add_buf(output,buf);
 sprintf(buf,"{&Form: {#%s{x\n\r", form_bit_name(race_table[race].form));
 add_buf(output,buf);
 sprintf(buf,"{&Parts: {#%s{x\n\r", part_bit_name(race_table[race].parts));
 add_buf(output,buf);
 if(race_table[race].pc_race)
 {
  strcpy(buf,"{&-------PLAYER RACE-------{x\n\r");
  add_buf(output,buf);
  sprintf(buf,"{&Points: {#%d{x\n\r", pc_race_table[pcrace].points);
  add_buf(output,buf);
  for(i=1;i < 5;i++)
  {
   if(pc_race_table[pcrace].skills[i] != '\0')
   {
    sprintf(buf,"{&Skill %d: {#%s{x\n\r", i, pc_race_table[pcrace].skills[0]);
    add_buf(output,buf);
   }
  }
  sprintf(buf,"{&Starting Stats: {#Str:{&({$%d{&){# Int:{&({$%d{&){# Wis:{&({$%d{&){# Dex:{&({$%d{&){# Con:{&({$%d{&){x\n\r",
  pc_race_table[pcrace].stats[0],pc_race_table[pcrace].stats[1],
  pc_race_table[pcrace].stats[2],pc_race_table[pcrace].stats[3],
  pc_race_table[pcrace].stats[4]);
  add_buf(output,buf);
  sprintf(buf,"{&Max Stats: {#Str:{&({$%d{&){# Int:{&({$%d{&){# Wis:{&({$%d{&){# Dex:{&({$%d{&){# Con:{&({$%d{&){x\n\r",
  pc_race_table[pcrace].max_stats[0],pc_race_table[pcrace].max_stats[1],
  pc_race_table[pcrace].max_stats[2],pc_race_table[pcrace].max_stats[3],
  pc_race_table[pcrace].max_stats[4]);
  add_buf(output,buf);
  sprintf(buf,"{&Size:{#%s{x\n\r", size_name(pc_race_table[pcrace].size));
  for(i=1; i < MAX_RARES;i++)
  {
   if(pc_race_table[pcrace].rare_race[i] != '\0')
   {
    sprintf(buf,"{&Evolves into: {#%s{x\n\r", pc_race_table[pcrace].rare_race[i]);
    add_buf(output,buf);
   }
  }
 }

 page_to_char (buf_string (output), ch);
 free_buf (output);
}

void do_classhelp(CHAR_DATA *ch, char *argument)
{
 char buf[MSL/4];
 int class;
 BUFFER *output;

 output = new_buf();

 class = class_lookup(argument);

 sprintf(buf,"{&---------- {!Class: {^%s{& ----------{x\n\r",class_table[class].name);
 add_buf(output,buf);
 sprintf(buf,"{&Prime Attribute: {#%s{x\n\r", attr_bit_name(class_table[class].attr_prime));
 add_buf(output,buf);
 sprintf(buf,"{&Starting Weapon: {#%s{x\n\r", start_wpn_bit_name(class_table[class].weapon));
 add_buf(output,buf);
 sprintf(buf,"{&Thac0 for Low Level: {#%d{x\n\r", class_table[class].thac0_00);
 add_buf(output,buf);
 sprintf(buf,"{&Thac0 for High Level: {#%d{x\n\r", class_table[class].thac0_32);
 add_buf(output,buf);
 sprintf(buf,"{&Hp Gain Range: {#%d{&-{#%d{x\n\r", class_table[class].hp_min,
 class_table[class].hp_max);
 add_buf(output,buf);
 sprintf(buf,"{&Class uses Magic: {#%s{x\n\r", (class_table[class].fMana ? "Yes" : "No"));
 add_buf(output,buf);
 sprintf(buf,"{&Class Completion Status: {#%s{x\n\r", class_fin_bit(class_table[class].finished));
 add_buf(output,buf);

 page_to_char (buf_string (output), ch);
 free_buf (output);
}

char * attr_bit_name(int attr)
{
 switch(attr)
 {
  case STAT_STR: return "Strength.";
  case STAT_INT: return "Intelligence.";
  case STAT_WIS: return "Wisdom.";
  case STAT_DEX: return "Dexterity.";
  case STAT_CON: return "Constitution.";
 }

 return "None.";
}

char * start_wpn_bit_name(int weapon_number)
{
 switch(weapon_number)
 {
  case OBJ_VNUM_SCHOOL_SWORD: return weapon_table[0].name;
  case OBJ_VNUM_SCHOOL_MACE: return weapon_table[1].name;
  case OBJ_VNUM_SCHOOL_DAGGER: return weapon_table[2].name;
  case OBJ_VNUM_SCHOOL_AXE: return weapon_table[3].name;
  case OBJ_VNUM_SCHOOL_STAFF: return weapon_table[4].name;
  case OBJ_VNUM_SCHOOL_FLAIL: return weapon_table[5].name;
  case OBJ_VNUM_SCHOOL_WHIP: return weapon_table[6].name;
  case OBJ_VNUM_SCHOOL_POLEARM: return weapon_table[7].name;
 }

 return "None.";
}

char * class_fin_bit(int bit)
{

 if(bit & NOT_STARTED) return "Not Started.";
 else if(bit & BASIC_START) return "Basic Structure Started.";
 else if(bit & SEMI_FINISHED) return "Semi-Finished.";
 else if(bit & FINISHED) return "Finished.";
 else if(bit & REVAMPING) return "Revamping.";

 return "Unavailable.";
}

/* race table */
const 	struct	race_type	race_table	[]		=
{
/*
    {
	name,		pc_race?,
	act bits,
	aff_by bits,
	off bits,
	imm,
	res,
	vuln,
	shd bits,
	form,		parts
    },
*/
    { "unique",		FALSE, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

    {
	"human",		TRUE,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "elf",                  TRUE,
	0,
        AFF_INFRARED,
	0,
        IMM_CHARM,
        0,
        0,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"dwarf",		TRUE,
	0,
        AFF_INFRARED,
	0,
	0,
        RES_POISON,
        0,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "hill-giant",                TRUE,
	0,
	0,
	0,
	0,
        0,
        VULN_MAGIC,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"halfling",		TRUE,
	0,
        AFF_INFRARED,
	0,
	0,
        0,
        0,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "orc",              TRUE,
        0,
        AFF_INFRARED,
	0,
        0,
	0,
        VULN_LIGHT,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "ogre",              TRUE,
        0,
	0,
	0,
        0,
	0,
	0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"goblin",		TRUE,
	0,
        AFF_INFRARED,
	0,
	0,
        0,
        VULN_LIGHT,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "high-elf",                  TRUE,
	0,
        AFF_INFRARED,
	0,
        IMM_CHARM,
        0,
        0,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },


    {
        "sylvan-elf",                TRUE,
        0,
        AFF_INFRARED,
        0,
        IMM_CHARM,
        0,
        0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "grey-elf",                     TRUE,
        0,
        AFF_INFRARED,
        0,
        IMM_CHARM,
        0,
        0,
	0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },


    {
        "derro",                TRUE,
	0,
        AFF_INFRARED,
	0,
	0,
        RES_POISON,
        VULN_LIGHT,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "duergar",                TRUE,
	0,
        AFF_INFRARED,
	0,
	0,
        RES_POISON,
        VULN_LIGHT,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "mountain-dwarf",                TRUE,
	0,
        AFF_INFRARED,
	0,
	0,
        RES_POISON,
        VULN_DROWNING,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },




    {
        "fire-giant",           TRUE,
        0,
        0,
        0,
        RES_FIRE,
        0,
        VULN_COLD,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "frost-giant",           TRUE,
        0,
        0,
        0,
        RES_COLD,
        0,
        VULN_FIRE,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "storm-giant",          TRUE,
        0,
        0,
        0,
        RES_LIGHTNING,
        0,
        VULN_MENTAL,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "jotun",                TRUE,
        0,
        0,
        0,
        0,
        0,
        VULN_MENTAL,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"pixie",		TRUE,
	0,
        AFF_FLYING,
	0,
	0,
        0,
        0,
        SHD_INVISIBLE,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|P
    },


    {
	"halforc",		TRUE,
	0,
        AFF_INFRARED,
	0,
	0,
        0,
	0,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "half-ogre",            TRUE,
        0,
        AFF_INFRARED,
        0,
        0,
        0,
        0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },


    {
	"halfelf",		TRUE,
	0,
        AFF_INFRARED,
        0,
	0,
	0,
	0,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"aarakocra",		TRUE,
	0,
        AFF_FLYING|AFF_FARSIGHT|AFF_DETECT_MOTION,
	0,
	0,
        0,
        0,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|P
    },

    {
	"gnome",		TRUE,
	0,
        AFF_INFRARED,
	0,
	0,
        0,
        0,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"draconian",		TRUE,
	0,
        AFF_FLYING,
	0,
        0,
        RES_BASH,
        VULN_PIERCE,
	0,
        A|H|M|X|cc,        A|B|C|D|E|F|G|H|I|J|K|P|Q
    },

    {
        "centaur",              TRUE,
        0,
	0,
	0,
        0,
	0,
	0,
        0,
        A|H|N|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "gnoll",                TRUE,
        0,
        0,
	0,
        0,
	0,
        0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|U|V
    },

    {
        "kenku",                TRUE,
        0,
        AFF_FLYING|AFF_FARSIGHT|AFF_DETECT_MOTION,
        0,
        0,
        0,
	0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|P
    },

    {
        "minotaur",             TRUE,
        0,
        AFF_INFRARED,
	0,
        0,
	0,
        0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|W
    },

    {
        "satyr",                TRUE,
        0,
        AFF_DETECT_MOTION|AFF_INFRARED,
        0,
        0,
        0,
        0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"kender",		TRUE,
    	0,
        AFF_INFRARED,
	0,
	0,
        0,
        0,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "dark-elf",                 TRUE,
	0,
        AFF_INFRARED,
	0,
	0,
        0,
        VULN_LIGHT,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "alaghi",              TRUE,
        0,
	0,
	0,
        0,
	0,
	0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|U|V|W
    },

    {
        "beastman",              TRUE,
        0,
	0,
	0,
        0,
        RES_MAGIC,
	0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "bugbear",              TRUE,
        0,
        AFF_INFRARED,
	0,
        0,
	0,
	0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "bullywug",              TRUE,
        0,
        AFF_SWIM,
     IMM_DROWNING,
        0,
	0,
	0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "gremlin",              TRUE,
        0,
        AFF_FLYING,
	0,
        0,
        0,
	0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|P
    },

    {
        "firbolg",              TRUE,
        0,
        AFF_DETECT_MAGIC,
	0,
        0,
	0,
	0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "voadkyn",              TRUE,
        0,
        AFF_INFRARED,
	0,
        IMM_CHARM,
	0,
	0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "flind",              TRUE,
        0,
	0,
	0,
        0,
	0,
	0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "hobgoblin",              TRUE,
        0,
        AFF_INFRARED,
        0,
        0,
	0,
	0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "kobold",              TRUE,
        0,
        AFF_INFRARED|AFF_DARK_VISION,
	0,
        0,
	0,
        VULN_LIGHT,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "lizardman",              TRUE,
        0,
        AFF_SWIM,
	0,
        0,
	0,
	0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|Q
    },

    {
        "mongrelman",              TRUE,
        0,
	0,
	0,
        0,
	0,
	0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },


    {
        "ogre-mage",              TRUE,
        0,
	0,
	0,
        0,
	0,
	0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },



    {
        "saurial-bladeback",              TRUE,
        0,
        AFF_INFRARED,
	0,
        0,
        RES_SOUND,
        VULN_POISON,
        0,
        H|M|V|X|cc,        A|B|C|D|E|F|G|H|I|J|K|Q|U
    },

    {
        "saurial-finhead",              TRUE,
        0,
        AFF_INFRARED,
	0,
        0,
        RES_SOUND,
        VULN_POISON,
        0,
        H|M|V|X|cc,        A|B|C|D|E|F|G|H|I|J|K|Q|U
    },

    {
        "saurial-flyer",              TRUE,
        0,
        AFF_FLYING|AFF_INFRARED,
	0,
        0,
        RES_SOUND,
        VULN_POISON,
        0,
        H|M|V|X|cc,        A|B|C|D|E|F|G|H|I|J|K|P|U
    },

    {
        "saurial-hornhead",              TRUE,
        0,
        AFF_INFRARED,
	0,
        0,
        RES_SOUND,
        VULN_POISON,
        0,
        H|M|V|X|cc,        A|B|C|D|E|F|G|H|I|J|K|Q|U|W
    },

    {
        "swanmay",              TRUE,
        0,
	0,
	0,
        0,
	0,
	0,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "wemic",              TRUE,
        0,
	0,
	0,
        0,
	0,
	0,
        0,
        A|H|N|V,        A|B|C|D|E|F|G|H|I|J|K
    },



    {
        "white-dwarf",                TRUE,
	0,
        AFF_INFRARED,
	0,
	0,
        RES_POISON,
        VULN_DROWNING,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "athasian",                TRUE,
	0,
	0,
	0,
	0,
        0,
        VULN_MENTAL,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },


    {
        "ogrillon",              TRUE,
        0,
        AFF_INFRARED,
	0,
        0,
	0,
        VULN_LIGHT,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "white-orc",              TRUE,
        0,
        AFF_INFRARED,
	0,
        0,
	0,
        VULN_LIGHT,
        0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },


/*
    {
        "theiwar",              FALSE,
	0,
	AFF_INFRARED,
	0,
	0,
	RES_CHARM|RES_COLD|RES_NEGATIVE,
	VULN_IRON|VULN_LIGHT|VULN_HOLY,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "changeling",           FALSE,
	0,
	AFF_DARK_VISION,
	0,
	0,
	RES_MAGIC|RES_POISON,
	VULN_COLD|VULN_DROWNING,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|Q|U|V
    },

    {
        "gryphon",              FALSE,
	0,
	AFF_INFRARED|AFF_DARK_VISION|AFF_FLYING|AFF_SNEAK,
	0,
	0,
	RES_MAGIC|RES_POISON,
	VULN_COLD|VULN_ACID|VULN_DROWNING,
	0,
	A|J|M|V|W,	A|B|C|D|E|F|G|H|I|J|K|P|Q|U
    },

    {
        "gryfalcon",            FALSE,
	0,
	AFF_INFRARED|AFF_SNEAK|AFF_FLYING,
	0,
	0,
	RES_CHARM|RES_POISON|RES_DISEASE,
	VULN_ACID|VULN_COLD,
	0,
	A|J|M|V|W,	A|B|C|D|E|F|G|H|I|J|K|P|Q|U
    },

    {
        "sea-elf",              FALSE,
	0,
	AFF_SWIM|AFF_DETECT_MOTION,
	0,
	0,
	RES_DROWNING|RES_COLD,
	VULN_IRON|VULN_FIRE,
	0,
	A|H|M|V|aa|cc,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "red-dragon",           FALSE,
	0,
	AFF_INFRARED|AFF_FLYING|AFF_DARK_VISION,
	0,
	IMM_FIRE|IMM_LIGHTNING,
	RES_CHARM|RES_MAGIC|RES_DISEASE,
	VULN_COLD|VULN_ACID,
	0,
	A|H|M|Z,	A|B|C|D|E|F|G|H|I|J|K|P|Q|U|W|X
    },

    {
        "silver-dragon",                FALSE,
	0,
	AFF_INFRARED|AFF_FLYING|AFF_DARK_VISION,
	0,
	IMM_COLD,
	RES_CHARM|RES_MAGIC|RES_DISEASE,
	VULN_FIRE|VULN_LIGHTNING,
	0,
	A|H|M|Z,	A|B|C|D|E|F|G|H|I|J|K|P|Q|U|W|X
    },

    {
        "drider",               FALSE,
	0,
	AFF_INFRARED|AFF_HASTE,
	0,
	IMM_POISON,
	RES_MAGIC|RES_NEGATIVE,
	VULN_IRON|VULN_LIGHT|VULN_HOLY,
	0,
	A|H|P|V,	A|B|C|D|E|F|K
    },

    */{
        "demon",                TRUE,
	0,
	AFF_INFRARED|AFF_PASS_DOOR,
	0,
	0,
	RES_MAGIC|RES_NEGATIVE,
	VULN_HOLY|VULN_LIGHT,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|Q
    },

    {
        "valkyrie",             TRUE,
	0,
	AFF_INFRARED|AFF_FLYING|AFF_REGENERATION,
	0,
	0,
	RES_CHARM|RES_HOLY|RES_LIGHT,
	VULN_MAGIC|VULN_NEGATIVE,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|P
    },

    {
        "cyclops",              TRUE,
	0,
	AFF_BERSERK,
	0,
	0,
	RES_FIRE|RES_COLD,
	VULN_MENTAL|VULN_LIGHTNING,
	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|J|K
    },

    /*{
        "tiegre",                  FALSE,
        0,
        AFF_INFRARED|AFF_DETECT_MOTION|AFF_REGENERATION,
        OFF_DODGE|OFF_FAST|OFF_CRUSH|OFF_DISARM|OFF_BASH,
        IMM_CHARM,
        RES_POISON|RES_DISEASE|RES_LIGHT,
        0,
        0,
        A|H|M|V,        A|B|C|D|E|F|H|J|K|Q|U|V
    },

*/

/*
  mobs races
*/
    {
	"bat",			FALSE,
	0,
	AFF_FLYING|AFF_DARK_VISION,
	OFF_DODGE|OFF_FAST,
	0,
	0,
	VULN_LIGHT,
	0,
	A|G|V,		A|C|D|E|F|H|J|K|P
    },

    {
	"bear",			FALSE,
	0,
	0,
	OFF_CRUSH|OFF_DISARM|OFF_BERSERK,
	0,
	RES_BASH|RES_COLD,
	0,
	0,
	A|G|V,		A|B|C|D|E|F|H|J|K|U|V
    },

    {
	"cat",			FALSE,
	0,
	AFF_DARK_VISION,
	OFF_FAST|OFF_DODGE,
	0,
	0,
	0,
	0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V
    },

    {
	"centipede",		FALSE,
	0,
	AFF_DARK_VISION,
	0,
	0,
	RES_PIERCE|RES_COLD,
	VULN_BASH,
	0,
 	A|B|G|O,		A|C|K
    },

    {
	"dog",			FALSE,
	0,
	0,
	OFF_FAST,
	0,
	0,
	0,
	0,
	A|G|V,		A|C|D|E|F|H|J|K|U|V
    },

    {
	"doll",			FALSE,
	0,
	0,
	0,
	IMM_COLD|IMM_POISON|IMM_HOLY|IMM_NEGATIVE|IMM_MENTAL|IMM_DISEASE|IMM_DROWNING,
	RES_BASH|RES_LIGHT,
	VULN_SLASH|VULN_FIRE|VULN_ACID|VULN_LIGHTNING|VULN_ENERGY,
	0,
	E|J|M|cc,	A|B|C|G|H|K
    },

    { 	"dragon", 		FALSE,
	0,
	AFF_INFRARED|AFF_FLYING,
	0,
	0,
	RES_FIRE|RES_BASH|RES_CHARM,
	VULN_PIERCE|VULN_COLD,
	0,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X
    },

    {
	"fido",			FALSE,
	0,
	0,
	OFF_DODGE|ASSIST_RACE,
	0,
	0,
	VULN_MAGIC,
	0,
	A|B|G|V,	A|C|D|E|F|H|J|K|Q|V
    },

    {
	"fox",			FALSE,
	0,
	AFF_DARK_VISION,
	OFF_FAST|OFF_DODGE,
	0,
	0,
	0,
	0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|V
    },

    {
	"lizard",		FALSE,
	0,
	0,
	0,
	0,
	RES_POISON,
	VULN_COLD,
	0,
	A|G|X|cc,	A|C|D|E|F|H|K|Q|V
    },

    {
	"pig",			FALSE,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	A|G|V,	 	A|C|D|E|F|H|J|K
    },

    {
	"rabbit",		FALSE,
	0,
	0,
	OFF_DODGE|OFF_FAST,
	0,
	0,
	0,
	0,
	A|G|V,		A|C|D|E|F|H|J|K
    },

    {
	"school monster",	FALSE,
	ACT_NOALIGN,
	0,
	0,
	IMM_CHARM|IMM_SUMMON,
	0,
	VULN_MAGIC,
	0,
	A|M|V,		A|B|C|D|E|F|H|J|K|Q|U
    },

    {
	"snake",		FALSE,
	0,
	0,
	0,
	0,
	RES_POISON,
	VULN_COLD,
	0,
	A|G|X|Y|cc,	A|D|E|F|K|L|Q|V|X
    },

    {
	"song bird",		FALSE,
	0,
	AFF_FLYING,
	OFF_FAST|OFF_DODGE,
	0,
	0,
	0,
	0,
	A|G|W,		A|C|D|E|F|H|K|P
    },

    {
	"troll",		FALSE,
	0,
	AFF_REGENERATION|AFF_INFRARED|AFF_DETECT_MOTION,
	OFF_BERSERK,
 	0,
	RES_CHARM|RES_BASH,
	VULN_FIRE|VULN_ACID,
	0,
	A|B|H|M|V,		A|B|C|D|E|F|G|H|I|J|K|U|V
    },

    {
	"water fowl",		FALSE,
	0,
	AFF_SWIM|AFF_FLYING,
	0,
	0,
	RES_DROWNING,
	0,
	0,
	A|G|W,		A|C|D|E|F|H|K|P
    },

    {
	"wolf",			FALSE,
	0,
	AFF_DARK_VISION,
	OFF_FAST|OFF_DODGE,
	0,
	0,
	0,
	0,
	A|G|V,		A|C|D|E|F|J|K|Q|V
    },

    {
	"wyvern",		FALSE,
	0,
	AFF_FLYING|AFF_DETECT_INVIS|AFF_DETECT_MOTION,
	OFF_BASH|OFF_FAST|OFF_DODGE,
	IMM_POISON,
	0,
	VULN_LIGHT,
	0,
	A|B|G|Z,		A|C|D|E|F|H|J|K|Q|V|X
    },

    { "unique",		FALSE, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

    {
	NULL, 0, 0, 0, 0, 0, 0
    }
};

const	struct	pc_race_type	pc_race_table	[]	=
{
    {
	"null race",	"",
	{ "" },
	0,
	{ 100, 100, 100, 100, 100, 100, 100,
	  100, 100, 100, 100, 100, 100, 100 },
	{ "" },
	{ 13, 13, 13, 13, 13 },
	{ 18, 18, 18, 18, 18 },
	0
    },

/*
    {
	"race name", 	short name,
	{ "rare races" },
	points,
	{ class multipliers },
	{ bonus skills },
	{ base stats },
	{ max stats },
	size
    },
*/
    {
        "human",        "Human ",
        { "minotaur", "swanmay", "centaur",
	 "halfelf", "athasian", "halforc", "half-ogre" },
	0,
	{ 100, 100, 100, 100, 100, 100, 100,
	  90, 90, 90, 90, 90, 90, 90,
	80, 80, 80,		80, 80, 80,
	80, 80, 80,		80, 80, 80,
	80, 80, 80,		80, 80, 80,
	80, 80, 80 },
	{ "" },
	{ 13, 13, 13, 13, 13 },
	{ 18, 18, 18, 18, 18 },
	SIZE_MEDIUM
    },

    {
        "elf",          "  Elf ",
        { "halfelf", "dark-elf", "grey-elf",
         "high-elf", "sylvan-elf", "", "" },
        4,
        { 98, 98, 105, 105, 98, 98, 105,
          93, 93, 100, 100, 93, 93, 100,
	78, 78, 78,		78, 78, 78,
	85, 85, 85,		85, 85, 85,
	78, 78, 78,		78, 78, 78,
	85, 85, 85 },
        { "sneak", "hide" },
        { 12, 14, 13, 14, 12 },
        { 17, 19, 18, 19, 17 },
        SIZE_MEDIUM
    },

    {
        "dwarf",        "Dwarf ",
        { "duergar", "derro", "mountain-dwarf",
         "white-dwarf", "draconian", "", "" },
        3,
        { 150, 120, 110, 95, 130, 150, 120,
          140, 100, 100, 85, 120, 140, 100,
	130, 130, 130,	90, 90, 90,
	90, 90, 90,		75, 75, 75,
	110, 110, 110,	130, 130, 130,
	90, 90, 90 },
        { "berserk" },
        { 14, 13, 13, 11, 14 },
        { 19, 18, 18, 16, 19 },
        SIZE_SMALL
    },

    {
        "hill-giant",        "HillGt",
        { "frost-giant", "fire-giant", "storm-giant",
         "jotun", "athasian", "voadkyn", "firbolg" },
        2,
        { 200, 200, 150, 95, 110, 200, 120,
          190, 190, 140, 80, 100, 190, 110,
	180, 180, 180,	180, 180, 180,
	130, 130, 130,	70, 70, 70,
	90, 90, 90,		180, 180, 180,
	100, 100, 100 },
        { "bash" },
        { 15, 11, 12, 13, 15 },
        { 20, 16, 17, 18, 20 },
	SIZE_LARGE
    },

    {
        "halfling",     "Hlflng",
        { "kender", "gnome", "pixie", "flind", "kenku", "", "" },
        3,
	{ 100, 100, 100, 100, 100, 100, 100,
	  90, 90, 90, 90, 90, 90, 90,
	80, 80, 80,		80, 80, 80,
	80, 80, 80,		80, 80, 80,
	80, 80, 80,		80, 80, 80,
	80, 80, 80 },
        { "sneak", "hide" },
        { 11, 13, 13, 15, 13 },
        { 16, 18, 18, 20, 18 },
	SIZE_SMALL
    },

     {
        "orc",          " Orc  ",
        { "halforc", "ogrillon", "white-orc", "minotaur", "", "", "" },
        2,
        { 200, 110, 95, 95, 200, 200, 100,
          190, 100, 85, 85, 190, 190, 90,
	180, 180, 180,	90, 90, 90,
	75, 75, 75,		75, 75, 75,
	180, 180, 180,	180, 180, 180,
	80, 80, 80 },
	{ "" },
        { 14, 13, 12, 13, 13 },
        { 19, 18, 17, 18, 18 },
	SIZE_MEDIUM

     },

     {
        "ogre",         " Ogre ",
        { "ogre-mage", "half-ogre", "ogrillon", "aarakocra", "", "", "" },

        3,
        { 200, 140, 200, 95, 200, 200, 95,
          190, 130, 190, 85, 190, 190, 85,
	180, 180, 180,	120, 120, 120,
	180, 180, 180,	75, 75, 75,
	180, 180, 180,	180, 180, 180,
	75, 75, 75 },
	{ "" },
        { 15, 11, 11, 13, 15 },
        { 20, 16, 16, 18, 20 },
        SIZE_LARGE
     },

    {
        "goblin",       "Goblin",
        { "bugbear", "hobgoblin", "gremlin", "kobold", "kenku", "", "" },
        2,
        { 200, 130, 98, 100, 120, 200, 100,
          190, 120, 88, 90, 110, 190, 90,
	180, 180, 180,	110, 110, 110,
	78, 78, 78,		80, 80, 80,
	100, 100, 100,	180, 180, 180,
	80, 80, 80 },
	{ "" },
        { 12, 12, 12, 15, 13 },
        { 17, 17, 17, 20, 18 },
	SIZE_SMALL
    },

    {
        "high-elf",          "HghElf",
        { "", "", "", "", "", "", "" },
        4,
        { 95, 100, 100, 100, 100, 120, 110,
          85, 90, 90, 90, 90, 110, 100,
	75, 75, 75,		80, 80, 80,
	80, 80, 80,		80, 80, 80,
	80, 80, 80,		100, 100, 100,
	90, 90, 90 },
        { "sneak", "hide" },
        { 13, 13, 13, 14, 12 },
        { 18, 18, 18, 19, 17 },
        SIZE_MEDIUM
    },

    {
        "sylvan-elf",       "SvnElf",
        { "", "", "", "", "", "", "" },
        6,
        { 100, 100, 100, 110, 95, 95, 120,
          90, 90, 90, 100, 85, 85, 100,
	80, 80, 80,		80, 80, 80,
	80, 80, 80,		90, 90, 90,
	75, 75, 75,		75, 75, 75,
	90, 90, 90 },
        { "sneak", "hide", "ambush" },
        { 12, 13, 14, 14, 12 },
        { 17, 18, 19, 19, 17 },
        SIZE_MEDIUM
    },

    {
        "grey-elf",         "GryElf",
        { "", "", "", "", "", "", "" },
        4,
        { 95, 95, 120, 110, 100, 95, 120,
          85, 85, 110, 110, 90, 85, 110,
	75, 75, 75,		75, 75, 75,
	100, 100, 100,	100, 100, 100,
	80, 80, 80,		75, 75, 75,
	100, 100, 100 },
        { "hide", "sneak" },
        { 10, 15, 16, 12, 12 },
        { 15, 20, 21, 17, 17 },
        SIZE_MEDIUM
    },


    {
        "derro",   "Derro ",
        { "", "", "", "", "", "", "" },
        2,
        { 140, 115, 115, 95, 140, 160, 120,
          130, 105, 105, 85, 130, 150, 110,
	120, 120, 120,	95, 95, 95,
	95, 95, 95,		75, 75, 75,
	120, 120, 120,	140, 140, 140,
	100, 100, 100 },
        { "berserk" },
        { 14, 12, 14, 11, 14 },
        { 19, 17, 19, 16, 19 },
        SIZE_SMALL
    },

    {
        "duergar",      "Duergr",
        { "", "", "", "", "", "", "" },
        2,
        { 130, 110, 110, 95, 110, 150, 100,
          120, 100, 100, 85, 100, 140, 90,
	110, 110, 110,	90, 90, 90,
	90, 90, 90,		75, 75, 75,
	90, 90, 90,		130, 130, 130,
	80, 80, 80 },
        { "berserk" },
        { 14, 13, 13, 11, 14 },
        { 19, 18, 18, 16, 19 },
        SIZE_SMALL
    },

    {
        "mountain-dwarf",       "MtnDwf",
        { "", "", "", "", "", "", "" },
        2,
        { 130, 120, 110, 95, 120, 150, 110,
          120, 110, 100, 85, 110, 140, 100,
	110, 110, 110,	100, 100, 100,
	90, 90, 90,		75, 75, 75,
	100, 100, 100,	130, 130, 130,
	90, 90, 90 },
        { "berserk" },
        { 14, 13, 13, 11, 14 },
        { 19, 18, 18, 16, 19 },

    },

    {
        "fire-giant",       "FireGt",
        { "", "", "", "", "", "", "" },
        8,
        { 200, 200, 150, 95, 110, 200, 120,
          190, 190, 140, 85, 100, 190, 110,
	180, 180, 180,	180, 180, 180,
	130, 130, 130,	75, 75, 75,
	90, 90, 90,		180, 180, 180,
	100, 100, 100 },
        { "bash", "dual wield" },
        { 17, 10, 12, 12, 15 },
        { 22, 15, 17, 17, 20 },
        SIZE_LARGE
    },

    {
        "frost-giant",          "FrstGt",
        { "", "", "", "", "", "", "" },
        6,
        { 200, 200, 150, 95, 110, 200, 120,
          190, 190, 140, 85, 100, 190, 110,
	180, 180, 180,	180, 180, 180,
	130, 130, 130,	75, 75, 75,
	90, 90, 90,		180, 180, 180,
	100, 100, 100 },
        { "bash", "dual wield" },
        { 16, 11, 11, 12, 16 },
        { 21, 16, 16, 17, 21 },
        SIZE_LARGE
    },

    {
        "storm-giant",          "StrmGt",
        { "", "", "", "", "", "", "" },
        6,
        { 120, 140, 200, 100, 100, 120, 100,
          110, 130, 190, 90, 90, 110, 90,
	100, 100, 100,	120, 120, 120,
	180, 180, 180,	80, 80, 80,
	80, 80, 80,		100, 100, 100,
	80, 80, 80 },
        { "bash" },
        { 13, 11, 11, 11, 13 },
        { 24, 16, 16, 16, 18 },
        SIZE_LARGE
    },

    {
        "jotun",                "Jotun ",
        { "", "", "", "", "", "", "" },
        3,
        { 200, 200, 140, 95, 120, 200, 100,
          190, 190, 130, 85, 110, 190, 90,
	180, 180, 180,	180, 180, 180,
	120, 120, 120,	75, 75, 75,
	100, 100, 100,	180, 180, 180,
	80, 80, 80 },
        { "bash", "dual wield" },
        { 18, 10, 12, 11, 14 },
        { 23, 15, 17, 16, 19 },
        SIZE_LARGE
    },

    {
        "pixie",        "Pixie ",
        { "", "", "", "", "", "", "" },
        10,
        { 200, 140, 95, 110, 110, 120, 140,
          190, 130, 85, 100, 100, 110, 130,
	180, 180, 180,	120, 120, 120,
	75, 75, 75,		90, 90, 90,
	90, 90, 90,		100, 100, 100,
	120, 120, 120 },
        { "backstab" },
        { 11, 14, 14, 16, 12 },
        { 16, 19, 19, 21, 17 },
	SIZE_TINY
    },

    {
        "halforc",      "HlfOrc",
        { "", "", "", "", "", "", "" },
        1,
        { 200, 130, 100, 100, 140, 200, 100,
          190, 120, 90, 90, 130, 190, 90,
	180, 180, 180,	110, 110, 110,
	80, 80, 80,		80, 80, 80,
	120, 120, 120,	180, 180, 180,
	80, 80, 80 },
	{ "" },
        { 14, 12, 12, 13, 14 },
        { 19, 17, 17, 18, 19 },
	SIZE_MEDIUM
    },

    {
        "half-ogre",    "HlfOgr",
        { "", "", "", "", "", "", "" },
        2,
        { 200, 150, 120, 100, 140, 200, 100,
          190, 140, 110, 90, 130, 190, 90,
	180, 180, 180,	130, 130, 130,
	100, 100, 100,	80, 80, 80,
	120, 120, 120,	180, 180, 180,
	80, 80, 80 },
	{ "" },
        { 15, 11, 11, 13, 15 },
        { 20, 16, 16, 18, 20 },
        SIZE_MEDIUM
    },

    {
        "halfelf",      "HlfElf",
        { "", "", "", "", "", "", "" },
        1,
	{ 100, 100, 100, 100, 100, 100, 100,
	  90, 90, 90, 90, 90, 90, 90,
	80, 80, 80,		80, 80, 80,
	80, 80, 80,		80, 80, 80,
	80, 80, 80,		80, 80, 80,
	80, 80, 80 },
	{ "" },
	{ 13, 13, 13, 13, 13 },
	{ 18, 18, 18, 18, 18 },
	SIZE_MEDIUM
    },

    {
        "aarakocra",        "Arkcra",
        { "", "", "", "", "", "", "" },
        8,
        { 200, 100, 100, 110, 120, 120, 150,
          190, 90, 90, 100, 110, 110, 140,
	180, 180, 180,	80, 80, 80,
	80, 80, 80,		90, 90, 90,
	100, 100, 100,	100, 100, 100,
	130, 130, 130 },
	{ "" },
        { 12, 13, 14, 14, 12 },
        { 17, 18, 19, 19, 17 },
	SIZE_LARGE
    },

    {
        "gnome",        "Gnome ",
        { "", "", "", "", "", "", "" },
        4,
        { 95, 120, 110, 140, 200, 200, 120,
          85, 110, 100, 130, 190, 190, 110,
	75, 75, 75,		100, 100, 100,
	90, 90, 90,		120, 120, 120,
	180, 180, 180,	180, 180, 180,
	100, 100, 100 },
	{ "" },
        { 11, 15, 15, 11, 13 },
        { 16, 20, 20, 16, 18 },
	SIZE_SMALL
    },

    {
        "draconian",    "Drcnin",
        { "", "", "", "", "", "", "" },
        7,
        { 140, 200, 140, 100, 110, 200, 100,
          130, 190, 130, 90, 100, 190, 90,
	120, 120, 120,	180, 180, 180,
	120, 120, 120,	80, 80, 80,
	90, 90, 90,		180, 180, 180,
	80, 80, 80 },
        { "fast healing" },
        { 14, 11, 12, 15, 13 },
        { 19, 16, 17, 20, 18 },
        SIZE_MEDIUM
    },

    {
        "centaur",      "Cntaur",
        { "", "", "", "", "", "", "" },
        1,
        { 110, 130, 200, 120, 95, 95, 140,
          100, 120, 190, 110, 85, 85, 130,
	90, 90, 90,		110, 110, 110,
	180, 180, 180,	100, 100, 100,
	75, 75, 75,		75, 75, 75,
	120, 120, 120 },
	{ "" },
        { 13, 14, 14, 15, 14 },
        { 18, 19, 19, 20, 19 },
	SIZE_LARGE
    },

    {
        "gnoll",        "Gnoll ",
        { "", "", "", "", "", "", "" },
        1,
        { 200, 130, 100, 100, 140, 200, 120,
          190, 120, 90, 90, 130, 190, 110,
	180, 180, 180,	110, 110, 110,
	80, 80, 80,		80, 80, 80,
	120, 120, 120,	180, 180, 180,
	100, 100, 100 },
	{ "" },
        { 14, 12, 12, 13, 14 },
        { 19, 17, 17, 18, 19 },
	SIZE_LARGE
    },

    {
        "kenku",        "Kenku ",
        { "", "", "", "", "", "", "" },
        7,
        { 200, 100, 100, 110, 120, 120, 150,
          190, 90, 90, 100, 110, 110, 140,
	180, 180, 180,	80, 80, 80,
	80, 80, 80,		90, 90, 90,
	100, 100, 100,	100, 100, 100,
	130, 130, 130 },
	{ "" },
        { 12, 13, 14, 14, 12 },
        { 17, 18, 19, 19, 17 },
        SIZE_MEDIUM
    },

    {
        "minotaur",     "Mntaur",
        { "", "", "", "", "", "", "" },
        4,
        { 130, 150, 120, 95, 110, 200, 130,
          120, 140, 110, 85, 100, 190, 120,
	110, 110, 110,	130, 130, 130,
	100, 100, 100,	75, 75, 75,
	90, 90, 90,		180, 180, 180,
	110, 110, 110 },
        { "track" },
        { 15, 11, 11, 13, 15 },
        { 20, 16, 16, 18, 20 },
	SIZE_LARGE
    },

    {
        "satyr",        "Satyr ",
        { "", "", "", "", "", "", "" },
        6,
        { 200, 200, 120, 100, 95, 100, 150,
          190, 190, 110, 90, 85, 90, 140,
	180, 180, 180,	180, 180, 180,
	100, 100, 100,	80, 80, 80,
	75, 75, 75,		80, 80, 80,
	130, 130, 130 },
        { "sneak", "hide" },
        { 13, 11, 13, 14, 14 },
        { 18, 16, 18, 19, 19 },
        SIZE_SMALL
    },

    {
        "kender",       "Kender",
        { "", "", "", "", "", "", "" },
        8,
        { 200, 110, 95, 110, 120, 100, 95,
          190, 100, 85, 100, 110, 90, 85,
	180, 180, 180,	90, 90, 90,
	75, 75, 75,		90, 90, 90,
	100, 100, 100,	80, 80, 80,
	75, 75, 75 },
        { "steal", "hide", "sneak", "pick lock" },
        { 11, 12, 12, 13, 14 },
        { 16, 17, 17, 21, 19 },
	SIZE_SMALL
    },

    {
        "dark-elf",         "DrkElf",
        { "", "", "", "", "", "", "" },
        4,
        { 100, 120, 100, 100, 130, 110, 100,
          90, 110, 90, 90, 120, 100, 90,
	80, 80, 80,		100, 100, 100,
	80, 80, 80,		80, 80, 80,
	110, 110, 110,	90, 90, 90,
	80, 80, 80 },
        { "hide", "sneak" },
        { 13, 13, 13, 14, 12 },
        { 18, 18, 18, 19, 17 },
	SIZE_MEDIUM
    },

    {
        "alaghi",           "Alaghi",
        { "", "", "", "", "", "", "" },
        2,
        { 200, 120, 200, 95, 200, 200, 200,
          190, 110, 190, 85, 190, 190, 190,
	180, 180, 180,	100, 100, 100,
	180, 180, 180,	75, 75, 75,
	180, 180, 180,	180, 180, 180,
	180, 180, 180 },
        { "hide", "sneak" },
        { 15, 11, 13, 11, 15 },
        { 20, 16, 18, 16, 20 },
        SIZE_LARGE
     },

     {
        "beastman",         "Bstman",
        { "", "", "", "", "", "", "" },
        6,
        { 200, 200, 110, 95, 105, 200, 100,
          190, 190, 100, 85, 95, 190, 90,
	180, 180, 180,	180, 180, 180,
	90, 90, 90,		75, 75, 75,
	85, 85, 85,		180, 180, 180,
	80, 80, 80 },
        { "hide", "sneak" },
        { 13, 11, 12, 15, 14 },
        { 18, 16, 17, 20, 19 },
	SIZE_MEDIUM

     },

     {
        "bugbear",          "Bgbear",
        { "", "", "", "", "", "", "" },
        3,
        { 200, 140, 110, 100, 110, 140, 120,
          190, 130, 100, 90, 100, 130, 110,
	180, 180, 180,	120, 120, 120,
	90, 90, 90,		80, 80, 80,
	90, 90, 90,		120, 120, 120,
	100, 100, 100 },
        { "hide", "sneak" },
        { 14, 12, 13, 12, 14 },
        { 19, 17, 18, 17, 19 },
        SIZE_LARGE

     },

     {
        "bullywug",         "Blywug",
        { "", "", "", "", "", "", "" },
        5,
        { 200, 150, 95, 100, 100, 200, 120,
          190, 140, 85, 90, 90, 190, 110,
	180, 180, 180,	130, 130, 130,
	75, 75, 75,		80, 80, 80,
	80, 80, 80,		180, 180, 180,
	100, 100, 100 },
        { "hide", "sneak" },
        { 13, 12, 13, 13, 14 },
        { 18, 17, 18, 18, 19 },
	SIZE_MEDIUM

     },

     {
        "gremlin",          "Grmlin",
        { "", "", "", "", "", "", "" },
        8,
        { 95, 200, 100, 200, 200, 100, 140,
          85, 190, 90, 190, 190, 90, 130,
	75, 75, 75,		180, 180, 180,
	80, 80, 80,		180, 180, 180,
	180, 180, 180,	80, 80, 80,
	120, 120, 120 },
	{ "" },
        { 8, 15, 16, 15, 12 },
        { 14, 20, 21, 20, 17 },
        SIZE_TINY

     },

     {
        "firbolg",          "Firblg",
        { "", "", "", "", "", "", "" },
        7,
        { 200, 120, 200, 95, 110, 200, 120,
          190, 110, 190, 85, 100, 190, 110,
	180, 180, 180,	100, 100, 100,
	180, 180, 180,	75, 75, 75,
	90, 90, 90,		180, 180, 180,
	100, 100, 100 },
        { "bash" },
        { 15, 13, 13, 11, 13 },
        { 20, 18, 18, 16, 18 },
        SIZE_LARGE

     },

     {
        "voadkyn",          "Vodkyn",
        { "", "", "", "", "", "", "" },
        5,
        { 130, 130, 100, 95, 95, 100, 140,
          120, 120, 90, 85, 85, 90, 130,
	110, 110, 110,	110, 110, 110,
	80, 80, 80,		75, 75, 75,
	75, 75, 75,		80, 80, 80,
	120, 120, 120 },
	{ "" },
        { 14, 13, 11, 15, 13 },
        { 19, 18, 16, 20, 18 },
	SIZE_MEDIUM

     },

     {
        "flind",        "Flind ",
        { "", "", "", "", "", "", "" },
        2,
        { 200, 120, 100, 95, 200, 140, 95,
          190, 110, 90, 85, 190, 130, 85,
	180, 180, 180,	100, 100, 100,
	80, 80, 80,		75, 75, 75,
	180, 180, 180,	120, 120, 120,
	75, 75, 75 },
	{ "" },
        { 14, 13, 12, 13, 13 },
        { 19, 18, 17, 18, 18 },
	SIZE_MEDIUM

     },

     {
        "hobgoblin",    "Hbgbln",
        { "", "", "", "", "", "", "" },
        2,
        { 200, 140, 95, 100, 200, 200, 100,
          190, 130, 85, 90, 190, 190, 90,
	180, 180, 180,	120, 120, 120,
	75, 75, 75,		80, 80, 80,
	180, 180, 180,	180, 180, 180,
	80, 80, 80 },
	{ "" },
        { 13, 12, 13, 13, 14 },
        { 18, 17, 18, 18, 19 },
	SIZE_MEDIUM

     },

     {
        "kobold",       "Kobold",
        { "", "", "", "", "", "", "" },
        5,
        { 200, 120, 95, 100, 200, 200, 110,
          190, 110, 85, 90, 190, 190, 100,
	180, 180, 180,	100, 100, 100,
	75, 75, 75,		80, 80, 80,
	180, 180, 180,	180, 180, 180,
	90, 90, 90 },
	{ "" },
        { 12, 13, 13, 15, 12 },
        { 17, 18, 18, 20, 17 },
        SIZE_SMALL

     },

     {
        "lizard-man",   "Lzrdmn",
        { "", "", "", "", "", "", "" },
        4,
        { 200, 140, 100, 95, 200, 200, 100,
          190, 130, 90, 85, 190, 190, 90,
	180, 180, 180,	120, 120, 120,
	80, 80, 80,		75, 75, 75,
	180, 180, 180,	180, 180, 180,
	80, 80, 80 },
	{ "" },
        { 13, 13, 13, 13, 15 },
        { 18, 17, 18, 18, 19 },
	SIZE_MEDIUM

     },

     {
        "mongrelman",   "Mgrlmn",
        { "", "", "", "", "", "", "" },
        3,
        { 110, 110, 95, 100, 200, 200, 100,
          100, 100, 85, 90, 190, 190, 90,
	90, 90, 90,		90, 90, 90,
	75, 75, 75,		80, 80, 80,
	180, 180, 180,	180, 180, 180,
	80, 80, 80 },
	{ "" },
        { 14, 12, 12, 13, 14 },
        { 19, 17, 17, 18, 19 },
	SIZE_MEDIUM

     },

     {
        "ogre-mage",    "OgrMge",
        { "", "", "", "", "", "", "" },
        4,
        { 95, 130, 110, 100, 200, 140, 95,
          85, 120, 100, 90, 190, 130, 85,
	75, 75, 75,		110, 110, 110,
	90, 90, 90,		80, 80, 80,
	180, 180, 180,	120, 120, 120,
	75, 75, 75 },
        { "" },
        { 14, 12, 12, 13, 14 },
        { 19, 17, 17, 18, 19 },
	SIZE_MEDIUM


     },




     {
        "saurial-bladeback", "Saur-BldBck",
        { "", "", "", "", "", "", "" },
        3,
        { 100, 95, 100, 100, 200, 100, 200,
          90, 85, 90, 90, 190, 90, 190,
	80, 80, 80,		75, 75, 75,
	80, 80, 80,		80, 80, 80,
	180, 180, 180,	80, 80, 80,
	180, 180, 180 },
	{ "" },
        { 13, 14, 14, 11, 13 },
        { 18, 19, 19, 16, 18 },
        SIZE_LARGE

     },

     {
        "saurial-finhead", "Saur-FinHed",
        { "", "", "", "", "", "", "" },
        3,
        { 100, 100, 100, 95, 95, 100, 200,
          90, 90, 90, 85, 85, 90, 190,
	80, 80, 80,		80, 80, 80,
	80, 80, 80,		75, 75, 75,
	75, 75, 75,		80, 80, 80,
	180, 180, 180 },
	{ "" },
	{ 13, 13, 13, 13, 13 },
	{ 18, 18, 18, 18, 18 },
        SIZE_MEDIUM

     },

     {
        "saurial-flyer", "Saur-Flyer ",
        { "", "", "", "", "", "", "" },
        6,
        { 100, 100, 95, 100, 200, 100, 200,
          90, 90, 85, 90, 190, 90, 190,
	80, 80, 80,		80, 80, 80,
	75, 75, 75,		80, 80, 80,
	180, 180, 180,	80, 80, 80,
	180, 180, 180 },
	{ "" },
        { 11, 13, 13, 15, 12 },
        { 16, 18, 18, 20, 17 },
        SIZE_SMALL

     },

     {
        "saurial-hornhead", "Saur-HrnHed",
        { "", "", "", "", "", "", "" },
        3,
        { 95, 100, 100, 100, 200, 100, 200,
          85, 90, 90, 90, 190, 90, 190,
	75, 75, 75,		80, 80, 80,
	80, 80, 80,		80, 80, 80,
	180, 180, 180,	80, 80, 80,
	180, 180, 180 },
	{ "" },
        { 14, 14, 13, 11, 13 },
        { 19, 19, 18, 16, 18 },
        SIZE_LARGE

     },

     {
        "swanmay",      "SwnMay",
        { "", "", "", "", "", "", "" },
        4,
        { 200, 200, 200, 200, 95, 95, 200,
          190, 190, 190, 190, 85, 85, 190,
	180, 180, 180,	180, 180, 180,
	180, 180, 180,	180, 180, 180,
	75, 75, 75,		75, 75, 75,
	180, 180, 180 },
	{ "" },
        { 13, 13, 14, 14, 13 },
        { 18, 18, 19, 19, 18 },
	SIZE_MEDIUM

     },

     {
        "wemic",        "   Wemic   ",
        { "", "", "", "", "", "", "" },
        3,
        { 200, 140, 140, 95, 200, 200, 200,
          190, 130, 130, 85, 190, 190, 190,
	180, 180, 180,	120, 120, 120,
	120, 120, 120,	75, 75, 75,
	180, 180, 180,	180, 180, 180,
	180, 180, 180 },
	{ "" },
        { 14, 13, 13, 12, 13 },
        { 19, 18, 18, 17, 18 },
	SIZE_MEDIUM

     },

    {
        "white-dwarf",       "WtDwrf",
        { "", "", "", "", "", "", "" },
        3,
        { 120, 90, 130, 115, 140, 110, 130,
          110, 80, 120, 105, 130, 100, 120,
	100, 100, 100,	70, 70, 70,
	110, 110, 110,		95, 95, 95,
	120, 120, 120,	90, 90, 90,
	110, 110, 110 },
        { "berserk" },
        { 12, 14, 14, 11, 14 },
        { 17, 19, 19, 16, 19 },

    },

    {
        "athasian",        "Athasn",
        { "", "", "", "", "", "", "" },
        4,
        { 190, 190, 140, 115, 130, 180, 130,
          180, 180, 130, 100, 120, 170, 120,
	170, 170, 170,	170, 170, 170,
	120, 120, 120,	90, 90, 90,
	110, 110, 110,		160, 160, 160,
	110, 110, 110 },
        { "bash" },
        { 14, 12, 13, 13, 14 },
        { 19, 17, 18, 18, 19 },
	SIZE_LARGE
    },

     {
        "ogrillon",          "Ogriln",
        { "", "", "", "", "", "", "" },
        3,
        { 200, 125, 145, 95, 200, 200, 97,
          190, 115, 135, 85, 190, 190, 87,
	180, 180, 180,	105, 105, 105,
	125, 125, 125,		75, 75, 75,
	180, 180, 180,	180, 180, 180,
	77, 77, 77 },
	{ "" },
        { 15, 12, 11, 13, 14 },
        { 20, 17, 16, 18, 17 },
	SIZE_MEDIUM
     },

     {
        "white-orc",          "WhtOrc",
        { "", "", "", "", "", "", "" },
        3,
        { 180, 90, 115, 115, 190, 190, 120,
          170, 80, 105, 105, 180, 180, 110,
	160, 160, 160,	70, 70, 70,
	95, 95, 95,	95, 95, 95,
	170, 170, 170,	170, 170, 170,
	100, 100, 100 },
	{ "meditation" },
        { 13, 13, 15, 12, 12 },
        { 18, 18, 20, 17, 17 },
	SIZE_MEDIUM
     },

     { NULL, "", { "" }, 0,
	{ 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
      0,0,0, 0,0,0, 0,0,0, 0,0,0,
      0,0,0, 0,0,0, 0,0,0 }, { "" },
  { 0,0,0,0,0 }, { 0,0,0,0,0 }, 0
     }

/*
    {
	"theiwar",	"Theiw",
	9,
	{ 125, 100,  110,  90, 100, 90, 120,
	  115,  93, 100,  85,  95,  85, 110,
	105, 105, 105,	83, 83, 83,
	90, 90, 90,		75, 75, 75,
	85, 85, 85,		75, 75, 75,
	100, 100, 100 },
	{ "sneak", "hide", "combat" },
	{ 12, 14, 13, 15, 11 },
	{ 16, 20, 17, 20, 17 },
	SIZE_SMALL
    },

    {
	"changeling",	"Chlng",
	7,
	{ 105, 120, 140,  95, 110, 100, 115,
	   95, 113, 130,  98, 100,  95, 104,
	85, 85, 85,		103, 103, 103,
	120, 120, 120,	88, 88, 88,
	90, 90, 90,		85, 85, 85,
	94, 94, 94 },
	{ "sneak", "hide", "illusion" },
	{ 12, 14, 13, 15, 11 },
	{ 17, 20, 19, 21, 16 },
	SIZE_MEDIUM
    },

    {
	"gryphon",	"Gryph",
	12,
	{ 195, 160, 110, 175, 110, 100, 100,
	  188, 153, 100, 158, 100,  90,  90,
	178, 178, 178,	143, 143, 143,
	90, 90, 90,		148, 148, 148,
	90, 90, 90,		80, 80, 80,
	80, 80, 80 },
	{ "gouge", "hide", "detection" },
	{ 14, 14, 13, 15, 11 },
	{ 20, 20, 17, 19, 21 },
	SIZE_LARGE
    },

    {
	"gryfalcon",	"Gryfa",
	13,
	{ 200, 115, 100, 150, 100, 100,  110,
	  190, 108,  95, 135,  95,  95,  100,
	180, 180, 180,	98, 98, 98,
	85, 85, 85,		125, 125, 125,
	85, 85, 85,		85, 85, 85,
	90, 90, 90 },
	{ "detect motion", "gouge" },
	{ 16, 13, 13, 11, 14 },
	{ 23, 18, 19, 16, 20 },
	SIZE_LARGE
    },

    {
	"sea-elf", 	"S-Elf",
	6,
	{ 125, 125, 110, 100, 110, 115, 115,
	  100, 113, 100,  90, 100, 100, 104,
	90, 90, 90,		103, 103, 103,
	90, 90, 90,		80, 80, 80,
	90, 90, 90,		90, 90, 90,
	94, 94, 94 },
	{ "sneak", "hide", "illusion" },
	{ 12, 14, 13, 15, 11 },
	{ 17, 20, 19, 21, 17 },
	SIZE_MEDIUM
    },

    {
	"red-dragon",	       "RDrag",
	18,
	{ 220, 220, 400, 300, 220, 220, 300,
	  210, 210, 390, 290, 210, 210, 290,
	200, 200, 200,	200, 200, 200,
	380, 380, 380,	280, 280, 280,
	200, 200, 200,	200, 200, 200,
	280, 280, 280 },
	{ "attack", "combat" },
	{ 17, 16, 16, 15, 18 },
	{ 23, 20, 20, 19, 23 },
	SIZE_HUGE
    },

    {
	"silver-dragon",	"SDrag",
	13,
	{ 400, 365, 300, 350, 310, 300, 330,
	  390, 340, 296, 335, 300, 396, 320,
	380, 380, 380,	330, 330, 330,
	286, 286, 286,	325, 325, 325,
	290, 290, 290,	386, 386, 386,
	310, 310, 310 },
	{ "attack", "combat", "healing" },
	{ 18, 16, 16, 17, 19 },
	{ 25, 21, 21, 20, 25 },
	SIZE_HUGE
    },

    {
	"drider",	"Dridr",
	8,
	{ 125,  95,  90, 100, 100,  90, 115,
	  110, 113,  90, 108,  95,  85, 104,
	100, 100, 100,	103, 103, 103,
	80, 80, 80,		98, 98, 98,
	85, 85, 85,		75, 75, 75,
	94, 94, 94 },
	{ "attack", "detection" },
	{ 12, 14, 13, 15, 11 },
	{ 20, 17, 17, 18, 22 },
	SIZE_MEDIUM
    },

    {
	"demon",	"Demon",
	13,
	{ 145, 125, 110, 100, 120, 100, 135,
	  125, 113, 100,  95, 105,  95, 124,
	115, 115, 115,	103, 103, 103,
	90, 90, 90,		85, 85, 85,
	95, 95, 95,		85, 85, 85,
	114, 114, 114 },
	{ "backstab", "combat" },
	{ 14, 14, 13, 15, 15 },
	{ 19, 20, 19, 18, 23 },
	SIZE_MEDIUM
    },

    {
	"valkyrie",	"Valky",
	13,
	{ 115, 165,  90, 100, 110, 95, 115,
	  108, 143,  86, 108,  95, 95, 104,
	98, 98, 98,		133, 133, 133,
	76, 76, 76,		107, 107, 107,
	85, 85, 85,		85, 85, 85,
	94, 94, 94 },
	{ "curative", "healing", "protection" },
	{ 14, 15, 16, 16, 15 },
	{ 18, 21, 21, 19, 20 },
	SIZE_MEDIUM
    },

    {
	"cyclops",	"Cyclp",
	9,
	{ 100, 100, 100, 160, 120, 110, 125,
	  100, 100, 100, 148, 115, 100, 114,
	90, 90, 90,		90, 90, 90,
	90, 90, 90,		138, 138, 138,
	105, 105, 105,	90, 90, 90,
	104, 104, 104 },
	{ "stun", "kick", "bash" },
 	{ 16, 12, 13, 13, 15 },
	{ 25, 16, 17, 18, 22 },
	SIZE_HUGE
    },

    {
        "tiegre",          "Tiegr",
        15,
        { 115, 128, 105, 100, 100, 105, 110,
          105, 118, 100,  95,  95, 100, 100,
	95, 95, 95,		108, 108, 108,
	90, 90, 90,		85, 85, 85,
	85, 85, 85,		90, 90, 90,
	90, 90, 90 },
        { "sneak", "hide", "second attack" },
        { 15, 14, 14, 15, 15 },
        { 21, 17, 17, 22, 21 },
        SIZE_LARGE
    },
*/
};




/*
 * Class table.
 */
const	struct	class_type	class_table	[MAX_CLASS]	=
{

/*
    {
	name, who, attr_prime, weapon,
	{guild1, guild2, guild3}, skill_adept thac0_00, thac0_32, hp_min, hp_max, mana?,
        base_group, default_group, finished_flags
    }
*/

    {
	"mage", "Mag",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
	{ 3018, 9618, 18113 },  75,  20, 6,  6,  8, TRUE,
        "mage basics", "mage default", FINISHED
    },

    {
	"cleric", "Cle",  STAT_WIS,  OBJ_VNUM_SCHOOL_MACE,
        { 3003, 9619, 5699 },  75,  20,  2,  7, 14, TRUE,
        "cleric basics", "cleric default", FINISHED
    },

    {
	"thief", "Thi",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3028, 9639, 5633 },  75,  20,  -4,  5, 11, FALSE,
        "thief basics", "thief default", FINISHED
    },

    {
	"warrior", "War",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
        { 3022, 9633, 5613 },  75,  20,  -10,  10, 16, FALSE,
        "warrior basics", "warrior default", FINISHED
    },

    {
	"ranger", "Ran",  STAT_STR,  OBJ_VNUM_SCHOOL_SPEAR,
        { 3372, 9752, 18111 },  75,  20,  -8,  8,  15, TRUE,
        "ranger basics", "ranger default", FINISHED
    },

    {
	"druid", "Dru",  STAT_WIS,  OBJ_VNUM_SCHOOL_POLEARM,
        { 3369, 9755, 18111 },  75,  20,  4,  7,  14,  TRUE,
        "druid basics", "druid default", FINISHED
    },

    {
        "shade", "Shd",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3375, 9758, 18113 },  75,  20, -6,  5,  11, TRUE,
        "shade basics", "shade default", SEMI_FINISHED
    },

    {
	"wizard", "Wiz",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
	{ 3018, 9618, 18113 },  75,  10, -4,  6,  18, TRUE,
        "wizard basics", "wizard default", FINISHED
    },

    {
	"priest", "Prs",  STAT_WIS,  OBJ_VNUM_SCHOOL_MACE,
        { 3003, 9619, 5699 },  75,  10, -2, 7, 24, TRUE,
        "priest basics", "priest default", FINISHED
    },

    {
	"mercenary", "Mer",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3028, 9639, 5633 },  75,  10,  -8,  5, 22, FALSE,
        "mercenary basics", "mercenary default", FINISHED
    },

    {
	"gladiator", "Gla",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
        { 3022, 9633, 5613 },  75,  10,  -20,  10, 26, FALSE,
        "gladiator basics", "gladiator default", FINISHED
    },

    {
	"strider", "Str",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3372, 9752, 18111 },  75,  10,  -16,  8,  25, TRUE,
        "strider basics", "strider default", FINISHED
    },

    {
	"sage", "Sag",  STAT_WIS,  OBJ_VNUM_SCHOOL_POLEARM,
        { 3369, 9755, 18111 },  75,  10,  -4,  7,  24,  TRUE,
        "sage basics", "sage default", SEMI_FINISHED
    },

    {
        "lich", "Lic",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3375, 9758, 18113 },  75,  10, -12,  5,  21, TRUE,
        "lich basics", "lich default", FINISHED
    },

    {
	"forsaken", "Fkn",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3018, 9618, 18113 },  75,  5, -12,  11,  28, TRUE,
        "forsaken basics", "forsaken default", SEMI_FINISHED
    },

    {
	"conjurer", "Cjr",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3018, 9618, 18113 },  75,  5, -12,  11,  28, TRUE,
        "conjurer basics", "conjurer default", SEMI_FINISHED
    },

    {
	"archmage", "Amg",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3018, 9618, 18113 },  75,  5, -12,  11,  28, TRUE,
        "archmage basics", "archmage default", SEMI_FINISHED
    },

    {
	"voodan", "Vdn",  STAT_WIS,  OBJ_VNUM_SCHOOL_MACE,
      { 3003, 9619, 5699 },  75,  5, -12, 10, 36, TRUE,
        "voodan basics", "voodan default", BASIC_START
    },

    {
	"monk", "Mnk",  STAT_WIS,  OBJ_VNUM_SCHOOL_MACE,
      { 3003, 9619, 5699 },  75,  5, -24, 10, 36, TRUE,
        "monk basics", "monk default", REVAMPING
    },

    {
	"saint", "Snt",  STAT_WIS,  OBJ_VNUM_SCHOOL_MACE,
      { 3003, 9619, 5699 },  75,  5, -10, 10, 36, TRUE,
        "saint basics", "saint default", FINISHED
    },

    {
	"assassin", "Asn",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3028, 9639, 5633 },  75,  5,  -24,  7, 33, FALSE,
        "assassin basics", "assassin default", SEMI_FINISHED
    },

    {
	"ninja", "Nja",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3028, 9639, 5633 },  75,  5,  -24,  7, 33, FALSE,
        "ninja basics", "ninja default", SEMI_FINISHED
    },

    {
	"bard", "Brd",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3028, 9639, 5633 },  75,  5,  -24,  7, 33, FALSE,
        "bard basics", "bard default", REVAMPING
    },

    {
	"highlander", "Hdr",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
        { 3022, 9633, 5613 },  75,  5,  -30,  15, 39, FALSE,
        "highlander basics", "highlander default", SEMI_FINISHED
    },

    {
	"swashbuckler", "Sbl",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
        { 3022, 9633, 5613 },  75,  5,  -30,  15, 39, FALSE,
        "swashbuckler", "swashbuckler default", SEMI_FINISHED
    },

    {
	"crusader", "Csr",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
        { 3022, 9633, 5613 },  75,  5,  -30,  15, 39, FALSE,
        "crusader basics", "crusader default", SEMI_FINISHED
    },

    {
	"darkpaladin", "Dpn",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3372, 9752, 18111 },  75,  5,  -24,  12,  37, TRUE,
        "darkpaladin basics", "darkpaladin default", SEMI_FINISHED
    },

    {
	"hunter", "Htr",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3372, 9752, 18111 },  75,  5,  -24,  12,  37, TRUE,
        "hunter basics", "hunter default", SEMI_FINISHED
    },

    {
	"paladin", "Pdn",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3372, 9752, 18111 },  75,  5,  -24,  12,  37, TRUE,
        "paladin basics", "paladin default", SEMI_FINISHED
    },

    {
	"warlock", "Wlk",  STAT_WIS,  OBJ_VNUM_SCHOOL_POLEARM,
        { 3369, 9755, 18111 },  75,  5,  -14,  10,  34,  TRUE,
        "warlock basics", "warlock default", SEMI_FINISHED
    },

    {
	"alchemist", "Amt",  STAT_WIS,  OBJ_VNUM_SCHOOL_POLEARM,
        { 3369, 9755, 18111 },  75,  5,  -14,  10,  34,  TRUE,
        "alchemist basics", "alchemist default", REVAMPING
    },

    {
	"shaman", "Shn",  STAT_WIS,  OBJ_VNUM_SCHOOL_POLEARM,
        { 3369, 9755, 18111 },  75,  5,  -14,  10,  34,  TRUE,
        "shaman basics", "shaman default", BASIC_START
    },

    {
        "fade", "Fde",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3375, 9758, 18113 },  75,  5, -30,  7,  31, TRUE,
        "fade basics", "fade default", SEMI_FINISHED
    },

    {
        "necromancer", "Ncr",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3375, 9758, 18113 },  75,  5, -30,  7,  31, TRUE,
        "necromancer basics", "necromancer default", REVAMPING
    },

    {
        "banshee", "Bns",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3375, 9758, 18113 },  75,  5, -30,  7,  31, TRUE,
        "banshee basics", "banshee default", SEMI_FINISHED
    }
};


/*
 * Titles.
 */
char *	const			title_table	[MAX_CLASS][MAX_LEVEL+1][2] =
{

/*	Mage	*/

    {
        { "Man",                        "Woman"                         },

        { "Novice of Magic",            "Novice of Magic"               },
        { "Apprentice of Magic",        "Apprentice of Magic"           },
        { "Spell Novice",               "Spell Novice"                  },
        { "Spell Student",              "Spell Student"                 },
        { "Scholar of Magic",           "Scholar of Magic"              },
        { "Scholar of Wizardry",        "Scholar of Wizardry"           },
        { "Delver in Scrolls",          "Delveress in Scrolls"          },
        { "Delver in Spells",           "Delveress in Spells"           },
        { "Medium of Magic",            "Medium of Magic"               },
        { "Medium of Wizardry",         "Medium of Wizardry"            },
        { "Scribe of Magic",            "Scribess of Magic"             },
        { "Scribe of Wizardry",         "Scribess of Wizardry"          },
        { "Minor Seer",                 "Minor Seeress"                 },
        { "Seer",                       "Seeress"                       },
        { "Minor Sage",                 "Minor Sage"                    },
        { "Sage",                       "Sage"                          },
        { "Minor Illusionist",          "Minor Illusionist"             },
        { "Illusionist",                "Illusionist"                   },
        { "Minor Abjurer",              "Minor Abjuress"                },
        { "Abjurer",                    "Abjuress"                      },
        { "Minor Invoker",              "Minor Invoker"                 },
        { "Invoker",                    "Invoker"                       },
        { "Minor Enchanter",            "Minor Enchantress"             },
        { "Enchanter",                  "Enchantress"                   },
        { "Minor Conjurer",             "Minor Conjuress"               },
        { "Conjurer",                   "Conjuress"                     },
        { "Minor Magician",             "Minor Witch"                   },
        { "Magician",                   "Witch"                         },
        { "Minor Creator",              "Minor Creator"                 },
        { "Creator",                    "Creator"                       },
        { "Minor Savant",               "Minor Savant"                  },
        { "Savant",                     "Savant"                        },
        { "Minor Magus",                "Minor Craftess"                },
        { "Magus",                      "Craftess"                      },
        { "Minor Wizard",               "Minor Wizard"                  },
        { "Wizard",                     "Wizard"                        },
        { "Minor Warlock",              "Minor War Witch"               },
        { "Warlock",                    "War Witch"                     },
        { "Minor Sorcerer",             "Minor Sorceress"               },
        { "Sorcerer",                   "Sorceress"                     },
        { "Elder Sorcerer",             "Elder Sorceress"               },
        { "Elder Sorcerer",             "Elder Sorceress"               },
        { "Grand Sorcerer",             "Grand Sorceress"               },
        { "Grand Sorcerer",             "Grand Sorceress"               },
        { "Great Sorcerer",             "Great Sorceress"               },
        { "Great Sorcerer",             "Great Sorceress"               },
        { "Golem Maker",                "Golem Maker"                   },
        { "Golem Maker",                "Golem Maker"                   },
        { "Greater Golem Maker",        "Greater Golem Maker"           },
        { "Greater Golem Maker",        "Greater Golem Maker"           },
        { "Maker of Stones",            "Maker of Stones",              },
        { "Maker of Stones",            "Maker of Stones",              },
        { "Maker of Potions",           "Maker of Potions",             },
        { "Maker of Potions",           "Maker of Potions",             },
        { "Maker of Scrolls",           "Maker of Scrolls",             },
        { "Maker of Scrolls",           "Maker of Scrolls",             },
        { "Maker of Wands",             "Maker of Wands",               },
        { "Maker of Wands",             "Maker of Wands",               },
        { "Maker of Staves",            "Maker of Staves",              },
        { "Maker of Staves",            "Maker of Staves",              },
        { "Demon Summoner",             "Demon Summoner"                },
        { "Demon Summoner",             "Demon Summoner"                },
        { "Greater Demon Summoner",     "Greater Demon Summoner"        },
        { "Greater Demon Summoner",     "Greater Demon Summoner"        },
        { "Dragon Charmer",             "Dragon Charmer"                },
        { "Dragon Charmer",             "Dragon Charmer"                },
        { "Greater Dragon Charmer",     "Greater Dragon Charmer"        },
        { "Greater Dragon Charmer",     "Greater Dragon Charmer"        },
        { "Master of all Magic",        "Master of all Magic"           },
        { "Master of all Magic",        "Master of all Magic"           },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Master Mage",                "Master Mage"                   },
        { "Mage Hero",                  "Mage Heroine"                  },
        { "Squire of Magic",            "Squire of Magic"               },
        { "Knight of Magic",            "Dame of Magic"			},
        { "Demigod of Magic",           "Demigoddess of Magic"          },
        { "Immortal of Magic",          "Immortal of Magic"             },
        { "God of Magic",               "Goddess of Magic"              },
        { "Deity of Magic",             "Deity of Magic"                },
        { "Supremity of Magic",         "Supremity of Magic"            },
        { "Creator",                    "Creator"                       },
        { "Supreme Deity",              "Supreme Deity"                 }
    },

/*	Cleric	*/

    {
	{ "Man",			"Woman"				},

	{ "Believer",			"Believer"			},
	{ "Believer",			"Believer"			},
	{ "Attendant",			"Attendant"			},
	{ "Attendant",			"Attendant"			},
	{ "Acolyte",			"Acolyte"			},
	{ "Acolyte",			"Acolyte"			},
	{ "Novice",			"Novice"			},
	{ "Novice",			"Novice"			},
	{ "Missionary",			"Missionary"			},
	{ "Missionary",			"Missionary"			},
	{ "Adept",			"Adept"				},
	{ "Adept",			"Adept"				},
	{ "Deacon",			"Deaconess"			},
	{ "Deacon",			"Deaconess"			},
	{ "Vicar",			"Vicaress"			},
	{ "Vicar",			"Vicaress"			},
	{ "Priest",			"Priestess"			},
	{ "Priest",			"Priestess"			},
	{ "Minister",			"Lady Minister"			},
	{ "Minister",			"Lady Minister"			},
	{ "Canon",			"Canon"				},
	{ "Canon",			"Canon"				},
	{ "Levite",			"Levitess"			},
	{ "Levite",			"Levitess"			},
	{ "Curate",			"Curess"			},
	{ "Curate",			"Curess"			},
	{ "Monk",			"Nun"				},
	{ "Monk",			"Nun"				},
	{ "Healer",			"Healess"			},
	{ "Healer",			"Healess"			},
	{ "Chaplain",			"Chaplain"			},
	{ "Chaplain",			"Chaplain"			},
	{ "Expositor",			"Expositress"			},
	{ "Expositor",			"Expositress"			},
	{ "Bishop",			"Bishop"			},
	{ "Bishop",			"Bishop"			},
	{ "Arch Bishop",		"Arch Lady of the Church"	},
	{ "Arch Bishop",		"Arch Lady of the Church"	},
	{ "Patriarch",			"Matriarch"			},
	{ "Patriarch",			"Matriarch"			},
	{ "Elder Patriarch",		"Elder Matriarch"		},
	{ "Elder Patriarch",		"Elder Matriarch"		},
	{ "Grand Patriarch",		"Grand Matriarch"		},
	{ "Grand Patriarch",		"Grand Matriarch"		},
	{ "Great Patriarch",		"Great Matriarch"		},
	{ "Great Patriarch",		"Great Matriarch"		},
	{ "Demon Killer",		"Demon Killer"			},
	{ "Demon Killer",		"Demon Killer"			},
	{ "Greater Demon Killer",	"Greater Demon Killer"		},
	{ "Greater Demon Killer",	"Greater Demon Killer"		},
	{ "Cardinal of the Sea",	"Cardinal of the Sea"		},
	{ "Cardinal of the Sea",	"Cardinal of the Sea"		},
	{ "Cardinal of the Earth",	"Cardinal of the Earth"		},
	{ "Cardinal of the Earth",	"Cardinal of the Earth"		},
	{ "Cardinal of the Air",	"Cardinal of the Air"		},
	{ "Cardinal of the Air",	"Cardinal of the Air"		},
	{ "Cardinal of the Ether",	"Cardinal of the Ether"		},
	{ "Cardinal of the Ether",	"Cardinal of the Ether"		},
	{ "Cardinal of the Heavens",	"Cardinal of the Heavens"	},
	{ "Cardinal of the Heavens",	"Cardinal of the Heavens"	},
	{ "Avatar of an Immortal",	"Avatar of an Immortal"		},
	{ "Avatar of an Immortal",	"Avatar of an Immortal"		},
	{ "Avatar of a Deity",		"Avatar of a Deity"		},
	{ "Avatar of a Deity",		"Avatar of a Deity"		},
	{ "Avatar of a Supremity",	"Avatar of a Supremity"		},
	{ "Avatar of a Supremity",	"Avatar of a Supremity"		},
	{ "Avatar of an Implementor",	"Avatar of an Implementor"	},
	{ "Avatar of an Implementor",	"Avatar of an Implementor"	},
	{ "Master of all Divinity",	"Mistress of all Divinity"	},
	{ "Master of all Divinity",	"Mistress of all Divinity"	},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Master Cleric",		"Master Cleric"			},
	{ "Holy Hero",			"Holy Heroine"			},
	{ "Holy Squire",		"Holy Squire"			},
	{ "Holy Knight",		"Holy Dame"			},
	{ "Demigod",			"Demigoddess",			},
	{ "Immortal",			"Immortal"			},
	{ "God",			"Goddess"			},
	{ "Deity",			"Deity"				},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",                    "Creator"                       },
	{ "Supreme Deity",		"Supreme Deity"			}
    },

/*	Thief	*/

    {
	{ "Man",			"Woman"				},

	{ "Pilferer",			"Pilferess"			},
	{ "Pilferer",			"Pilferess"			},
	{ "Footpad",			"Footpad"			},
	{ "Footpad",			"Footpad"			},
	{ "Filcher",			"Filcheress"			},
	{ "Filcher",			"Filcheress"			},
	{ "Pick-Pocket",		"Pick-Pocket"			},
	{ "Pick-Pocket",		"Pick-Pocket"			},
	{ "Sneak",			"Sneak"				},
	{ "Sneak",			"Sneak"				},
	{ "Pincher",			"Pincheress"			},
	{ "Pincher",			"Pincheress"			},
	{ "Cut-Purse",			"Cut-Purse"			},
	{ "Cut-Purse",			"Cut-Purse"			},
	{ "Snatcher",			"Snatcheress"			},
	{ "Snatcher",			"Snatcheress"			},
	{ "Sharper",			"Sharpress"			},
	{ "Sharper",			"Sharpress"			},
	{ "Rogue",			"Rogue"				},
	{ "Rogue",			"Rogue"				},
	{ "Robber",			"Robber"			},
	{ "Robber",			"Robber"			},
	{ "Magsman",			"Magswoman"			},
	{ "Magsman",			"Magswoman"			},
	{ "Highwayman",			"Highwaywoman"			},
	{ "Highwayman",			"Highwaywoman"			},
	{ "Burglar",			"Burglaress"			},
	{ "Burglar",			"Burglaress"			},
	{ "Thief",			"Thief"				},
	{ "Thief",			"Thief"				},
	{ "Knifer",			"Knifer"			},
	{ "Knifer",			"Knifer"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "Killer",			"Murderess"			},
	{ "Killer",			"Murderess"			},
	{ "Brigand",			"Brigand"			},
	{ "Brigand",			"Brigand"			},
	{ "Cut-Throat",			"Cut-Throat"			},
	{ "Cut-Throat",			"Cut-Throat"			},
	{ "Spy",			"Spy"				},
	{ "Spy",			"Spy"				},
	{ "Grand Spy",			"Grand Spy"			},
	{ "Grand Spy",			"Grand Spy"			},
	{ "Master Spy",			"Master Spy"			},
	{ "Master Spy",			"Master Spy"			},
	{ "Assassin",			"Assassin"			},
	{ "Assassin",			"Assassin"			},
	{ "Greater Assassin",		"Greater Assassin"		},
	{ "Greater Assassin",		"Greater Assassin"		},
	{ "Master of Vision",		"Mistress of Vision"		},
	{ "Master of Vision",		"Mistress of Vision"		},
	{ "Master of Hearing",		"Mistress of Hearing"		},
	{ "Master of Hearing",		"Mistress of Hearing"		},
	{ "Master of Smell",		"Mistress of Smell"		},
	{ "Master of Smell",		"Mistress of Smell"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Touch",		"Mistress of Touch"		},
	{ "Master of Touch",		"Mistress of Touch"		},
	{ "Crime Lord",			"Crime Mistress"		},
	{ "Crime Lord",			"Crime Mistress"		},
	{ "Infamous Crime Lord",	"Infamous Crime Mistress"	},
	{ "Infamous Crime Lord",	"Infamous Crime Mistress"	},
	{ "Greater Crime Lord",		"Greater Crime Mistress"	},
	{ "Greater Crime Lord",		"Greater Crime Mistress"	},
	{ "Master Crime Lord",		"Master Crime Mistress"		},
	{ "Master Crime Lord",		"Master Crime Mistress"		},
	{ "Godfather",			"Godmother"			},
	{ "Godfather",			"Godmother"			},
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
        { "Master Thief",               "Master Thief"                  },
	{ "Assassin Hero",		"Assassin Heroine"		},
	{ "Squire of Death",		"Squire of Death",		},
	{ "Knight of Death",		"Dame of Death"			},
	{ "Demigod of Assassins",	"Demigoddess of Assassins"	},
	{ "Immortal Assasin",		"Immortal Assassin"		},
	{ "God of Assassins",		"God of Assassins",		},
	{ "Deity of Assassins",		"Deity of Assassins"		},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",                    "Creator"                       },
	{ "Supreme Deity",		"Supreme Deity"			}
    },

/*	Warrior	*/

    {
	{ "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},
	{ "Soldier",			"Soldier"			},
	{ "Warrior",			"Warrior"			},
	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},
	{ "Combatant",			"Combatess"			},
	{ "Hero",			"Heroine"			},
	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},
	{ "Swordmaster",		"Swordmistress"			},
	{ "Lieutenant",			"Lieutenant"			},
	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},
	{ "Knight",			"Lady Knight"			},
	{ "Grand Knight",		"Grand Knight"			},
	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Strider",			"Strider"			},
	{ "Strider",			"Strider"			},
	{ "Grand Strider",		"Grand Strider"			},
	{ "Grand Strider",		"Grand Strider"			},
	{ "Demon Slayer",		"Demon Slayer"			},
	{ "Demon Slayer",		"Demon Slayer"			},
	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},
	{ "Overlord",			"Overlord"			},
	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Meteors",		"Baroness of Meteors"		},
	{ "Baron of Meteors",		"Baroness of Meteors"		},
	{ "Master Warrior",		"Master Warrior"		},
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
	{ "Master Warrior",		"Master Warrior"		},
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
        { "Master Warrior",             "Master Warrior"                },
	{ "Knight Hero",		"Knight Heroine"		},
	{ "Squire of War",		"Squire of War"			},
	{ "Knight of War",		"Dame of War"			},
	{ "Demigod of War",		"Demigoddess of War"		},
	{ "Immortal Warlord",		"Immortal Warlord"		},
	{ "God of War",			"God of War"			},
	{ "Deity of War",		"Deity of War"			},
	{ "Supreme Master of War",	"Supreme Mistress of War"	},
        { "Creator",                    "Creator"                       },
	{ "Supreme Deity",		"Supreme Deity"			}
    },

/*	Ranger	*/

    {
	{ "Man",			"Woman"				},

	{ "Apprentice of the Hunt",	"Apprentice of the Hunt"	},
	{ "Student of the Hunt",	"Student of the Hunt"		},
	{ "Student of the Hunt",	"Student of the Hunt"		},
	{ "Runner",			"Runner"			},
	{ "Runner",			"Runner"			},
	{ "Strider",			"Strider"			},
	{ "Strider",			"Strider"			},
	{ "Scout",			"Scout"				},
	{ "Scout",			"Scout"				},
	{ "Master Scout",		"Master Scout"			},
	{ "Master Scout",		"Master Scout"			},
	{ "Explorer",			"Explorer"			},
	{ "Explorer",			"Explorer"			},
	{ "Tracker",			"Tracker"			},
	{ "Tracker",			"Tracker"			},
	{ "Survivalist",		"Survivalist"			},
	{ "Survivalist",		"Survivalist"			},
	{ "Watcher",			"Watcher"			},
	{ "Watcher",			"Watcher"			},
	{ "Hunter",			"Hunter"			},
	{ "Hunter",			"Hunter"			},
	{ "Woodsman",			"Woodswoman"			},
	{ "Woodsman",			"Woodswoman"			},
	{ "Guide",			"Guide"				},
	{ "Guide",			"Guide"				},
	{ "Pathfinder",			"Pathfinder"			},
	{ "Pathfinder",			"Pathfinder"			},
	{ "Stalker",			"Stalker"			},
	{ "Stalker",			"Stalker"			},
	{ "Predator",			"Predator"			},
	{ "Predator",			"Predator"			},
	{ "Deerhunter",			"Deerhunter"			},
	{ "Deerhunter",			"Deerhunter"			},
	{ "Elkhunter",			"Elkhunter"			},
	{ "Elkhunter",			"Elkhunter"			},
	{ "Boarhunter",			"Boarhunter"			},
	{ "Boarhunter",			"Boarhunter"			},
	{ "Bearhunter",			"Bearhunter"			},
	{ "Bearhunter",			"Bearhunter"			},
	{ "Falconer",			"Falconer"			},
	{ "Falconer",			"Falconer"			},
	{ "Hawker",			"Hawker"			},
	{ "Hawker",			"Hawker"			},
	{ "Manhunter",			"Manhunter"			},
	{ "Manhunter",			"Manhunter"			},
	{ "Gianthunter",		"Gianthunter"			},
	{ "Gianthunter",		"Gianthunter"			},
	{ "Adept of the Hunt",		"Adept of the Hunt"		},
	{ "Adept of the Hunt",		"Adept of the Hunt"		},
	{ "Lord of the Hunt",		"Lady of the Hunt"		},
	{ "Lord of the Hunt",		"Lady of the Hunt"		},
	{ "Master of the Hunt",		"Mistress of the Hunt"		},
	{ "Master of the Hunt",		"Mistress of the Hunt"		},
	{ "Fox",			"Fox"				},
	{ "Coyote",			"Coyote"			},
	{ "Lynx",			"Lynx"				},
	{ "Wolf",			"Wolf"				},
	{ "Panther",			"Panther"			},
	{ "Cougar",			"Cougar"			},
	{ "Jaguar",			"Jaguar"			},
	{ "Tiger",			"Tiger"				},
	{ "Lion",			"Lion"				},
	{ "Bear",			"Bear"				},
	{ "Beastmaster",		"Beastmaster"			},
	{ "Beastmaster",		"Beastmaster"			},
	{ "Overlord of Beasts",		"Overlord of Beasts"		},
	{ "Overlord of Beasts",		"Overlord of Beasts"		},
	{ "Dragonhunter",		"Dragonhunter"			},
	{ "Dragonhunter",		"Dragonhunter"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Master Ranger",		"Master Ranger"			},
	{ "Ranger Hero",		"Ranger Heroine"		},
	{ "Squire of the Hunt",		"Squire of the Hunt"		},
	{ "Knight of the Hunt",		"Dame of the Hunt"		},
	{ "Demigod of Beasts",		"Demigoddess of Beasts"		},
	{ "Immortal Hunter",		"Immortal Huntress"		},
	{ "God of Beasts",		"Goddess of Beasts"		},
	{ "Deity of Beasts",		"Deity of Beasts"		},
	{ "Supremity of Beasts",	"Supremity of Beasts"		},
	{ "Creator",			"Creator"			},
	{ "Supreme Deity",		"Supreme Deity"			},
    },

/*	Druid	*/

    {
	{ "Man",			"Woman"				},

	{ "Apprentice of Nature",	"Apprentice of Nature"		},
	{ "Student of Nature",		"Student of Nature"		},
	{ "Student of Nature",		"Student of Nature"		},
	{ "Scholar of Nature",		"Scholar of Nature"		},
	{ "Scholar of Nature",		"Scholar of Nature"		},
	{ "Neophyte",			"Neophyte"			},
	{ "Neophyte",			"Neophyte"			},
	{ "Naturalist",			"Naturalist"			},
	{ "Naturalist",			"Naturalist"			},
	{ "Forester",			"Forestress"			},
	{ "Forester",			"Forestress"			},
	{ "Student of Earth",		"Student of Earth"		},
	{ "Student of Earth",		"Student of Earth"		},
	{ "Student of Air",		"Student of Air"		},
	{ "Student of Air",		"Student of Air"		},
	{ "Student of Water",		"Student of Water"		},
	{ "Student of Water",		"Student of Water"		},
	{ "Student of Fire",		"Student of Fire"		},
	{ "Student of Fire",		"Student of Fire"		},
	{ "Student of Life",		"Student of Life"		},
	{ "Student of Life",		"Student of Life"		},
	{ "Student of The Elements",	"Student of The Elements"	},
	{ "Student of The Elements",	"Student of The Elements"	},
	{ "Herbalist",			"Herbalist"			},
	{ "Herbalist",			"Herbalist"			},
	{ "Philosopher",		"Philosopher"			},
	{ "Philosopher",		"Philosopher"			},
	{ "Sage",			"Sage"				},
	{ "Sage",			"Sage"				},
	{ "Prophet",			"Prophetess"			},
	{ "Prophet",			"Prophetess"			},
	{ "Adept of Earth",		"Adept of Earth"		},
	{ "Adept of Earth",		"Adept of Earth"		},
	{ "Adept of Air",		"Adept of Air"			},
	{ "Adept of Air",		"Adept of Air"			},
	{ "Adept of Water",		"Adept of Water"		},
	{ "Adept of Water",		"Adept of Water"		},
	{ "Adept of Fire",		"Adept of Fire"			},
	{ "Adept of Fire",		"Adept of Fire"			},
	{ "Adept of Life",		"Adept of Life"			},
	{ "Adept of Life",		"Adept of Life"			},
	{ "Adept of The Elements",	"Adept of The Elements"		},
	{ "Adept of The Elements",	"Adept of The Elements"		},
	{ "Druid of Earth",		"Druidess of Earth"		},
	{ "Druid of Earth",		"Druidess of Earth"		},
	{ "Druid of Air",		"Druidess of Air"		},
	{ "Druid of Air",		"Druidess of Air"		},
	{ "Druid of Water",		"Druidess of Water"		},
	{ "Druid of Water",		"Druidess of Water"		},
	{ "Druid of Fire",		"Druidess of Fire"		},
	{ "Druid of Fire",		"Druidess of Fire"		},
	{ "Druid of Life",		"Druidess of Life"		},
	{ "Druid of Life",		"Druidess of Life"		},
	{ "Druid of The Elements",	"Druidess of The Elements"	},
	{ "Druid of The Elements",	"Druidess of The Elements"	},
	{ "Sage of Earth",		"Sage of Earth"			},
	{ "Sage of Earth",		"Sage of Earth"			},
	{ "Sage of Air",		"Sage of Air"			},
	{ "Sage of Air",		"Sage of Air"			},
	{ "Sage of Water",		"Sage of Water"			},
	{ "Sage of Water",		"Sage of Water"			},
	{ "Sage of Fire",		"Sage of Fire"			},
	{ "Sage of Fire",		"Sage of Fire"			},
	{ "Sage of Life",		"Sage of Life"			},
	{ "Sage of Life",		"Sage of Life"			},
	{ "Sage of The Elements",	"Sage of The Elements"		},
	{ "Sage of The Elements",	"Sage of The Elements"		},
	{ "Master of Earth",		"Mistress of Earth"		},
	{ "Master of Earth",		"Mistress of Earth"		},
	{ "Master of Air",		"Mistress of Air"		},
	{ "Master of Air",		"Mistress of Air"		},
	{ "Master of Water",		"Mistress of Water"		},
	{ "Master of Water",		"Mistress of Water"		},
	{ "Master of Fire",		"Mistress of Fire"		},
	{ "Master of Fire",		"Mistress of Fire"		},
	{ "Master of Life",		"Mistress of Life"		},
	{ "Master of Life",		"Mistress of Life"		},
	{ "Master of The Elements",	"Mistress of The Elements"	},
	{ "Master of The Elements",	"Mistress of The Elements"	},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Master Druid",		"Master Druid"			},
	{ "Druid Hero",			"Druid Heroine"			},
	{ "Squire of The Elements",	"Squire of The Elements"	},
	{ "Knight of The Elements",	"Dame of The Elements"		},
	{ "Demigod of Nature",		"Demigoddess of Nature"		},
	{ "Immortal Sage",		"Immortal Sage"			},
	{ "God of The Elements",	"Goddess of The Elements"	},
	{ "Deity of The Elements",	"Deity of The Elements"		},
	{ "Supremity of The Elements",	"Supremity of The Elements"	},
	{ "Creator",			"Creator"			},
	{ "Supreme Diety",		"Supreme Diety"			},
    },

/*	Vampire	*/

    {
	{ "Man",			"Woman"				},

	{ "Blood Student",		"Blood Student"			},
	{ "Scholar of Blood",		"Scholar of Blood"		},
	{ "Scholar of Blood",		"Scholar of Blood"		},
	{ "Student of the Night",	"Student of the Night"		},
	{ "Student of the Night",	"Student of the Night"		},
	{ "Student of Death",		"Student of Death"		},
	{ "Student of Death",		"Student of Death"		},
	{ "Lesser Imp",			"Lesser Imp"			},
	{ "Greater Imp",		"Greater Imp"			},
	{ "Illusionist",		"Illusionist"			},
	{ "Illusionist",		"Illusionist"			},
	{ "Seducer",			"Seductress"			},
	{ "Seducer",			"Seductress"			},
	{ "Stalker",			"Stalker"			},
	{ "Stalker",			"Stalker"			},
	{ "Stalker of Blood",		"Stalker of Blood"		},
	{ "Stalker of Blood",		"Stalker of Blood"		},
	{ "Stalker of Night",		"Stalker of Night"		},
	{ "Stalker of Night",		"Stalker of Night"		},
	{ "Stalker of Death",		"Stalker of Death"		},
	{ "Stalker of Death",		"Stalker of Death"		},
	{ "Shadow",			"Shadow"			},
	{ "Shadow",			"Shadow"			},
	{ "Shadow of Blood",		"Shadow of Blood"		},
	{ "Shadow of Blood",		"Shadow of Blood"		},
	{ "Shadow of Night",		"Shadow of Night"		},
	{ "Shadow of Night",		"Shadow of Night"		},
	{ "Shadow of Death",		"Shadow of Death"		},
	{ "Shadow of Death",		"Shadow of Death"		},
	{ "Killer",			"Killer"			},
	{ "Killer",			"Killer"			},
	{ "Murderer",			"Murderer"			},
	{ "Murderer",			"Murderer"			},
	{ "Incubus",			"Succubus"			},
	{ "Incubus",			"Succubus"			},
	{ "Nightstalker",		"Nightstalker"			},
	{ "Nightstalker",		"Nightstalker"			},
	{ "Revenant",			"Revenant"			},
	{ "Revenant",			"Revenant"			},
	{ "Lesser Vrolok",		"Lesser Vrolok"			},
	{ "Vrolok",			"Vrolok"			},
	{ "Greater Vrolok",		"Greater Vrolok"		},
	{ "Demon Servant",		"Demon Servant"			},
	{ "Demon Servant",		"Demon Servant"			},
	{ "Adept of Blood",		"Adept of Blood"		},
	{ "Adept of Blood",		"Adept of Blood"		},
	{ "Adept of Night",		"Adept of Night"		},
	{ "Adept of Night",		"Adept of Night"		},
	{ "Adept of Death",		"Adept of Death"		},
	{ "Adept of Death",		"Adept of Death"		},
	{ "Demon Seeker",		"Demon Seeker"			},
	{ "Demon Seeker",		"Demon Seeker"			},
	{ "Lord of Blood",		"Lady of Blood"			},
	{ "Lord of Blood",		"Lady of Blood"			},
	{ "Lord of Night",		"Lady of Night"			},
	{ "Lord of Night",		"Lady of Night"			},
	{ "Lord of Death",		"Lady of Death"			},
	{ "Lord of Death",		"Lady of Death"			},
	{ "Demon Knight",		"Demon Dame"			},
	{ "Demon Knight",		"Demon Dame"			},
	{ "Master of Blood",		"Mistress of Blood"		},
	{ "Master of Blood",		"Mistress of Blood"		},
	{ "Master of Night",		"Mistress of Night"		},
	{ "Master of Night",		"Mistress of Night"		},
	{ "Master of Death",		"Mistress of Death"		},
	{ "Master of Death",		"Mistress of Death"		},
	{ "Lord of Demons",		"Lady of Demons"		},
	{ "Lord of Demons",		"Lady of Demons"		},
	{ "Baron of Demons",		"Baroness of Demons"		},
	{ "Baron of Demons",		"Baroness of Demons"		},
	{ "Master of Demons",		"Mistress of Demons"		},
	{ "Master of Demons",		"Mistress of Demons"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Master Vampire",		"Master Vampire"		},
	{ "Vampire Hero",		"Vampire Heroine"		},
	{ "Squire of Blood",		"Squire of Blood"		},
	{ "Knight of Blood",		"Dame of Blood"			},
	{ "Demigod of the Night",	"Demigoddess of the Night"	},
	{ "Immortal Undead",		"Immortal Undead"		},
	{ "God of Death",		"Goddess of Death"		},
	{ "Deity of Death",		"Deity of Death"		},
	{ "Supremity of Death",		"Supremity of Death"		},
	{ "Creator",			"Creator"			},
	{ "Supreme Diety",		"Supreme Deity"			},
    },

/*	Wizard	*/

    {
        { "Man",                        "Woman"                         },

        { "Novice of Magic",            "Novice of Magic"               },
        { "Apprentice of Magic",        "Apprentice of Magic"           },
        { "Spell Novice",               "Spell Novice"                  },
        { "Spell Student",              "Spell Student"                 },
        { "Scholar of Magic",           "Scholar of Magic"              },
        { "Scholar of Wizardry",        "Scholar of Wizardry"           },
        { "Delver in Scrolls",          "Delveress in Scrolls"          },
        { "Delver in Spells",           "Delveress in Spells"           },
        { "Medium of Magic",            "Medium of Magic"               },
        { "Medium of Wizardry",         "Medium of Wizardry"            },
        { "Scribe of Magic",            "Scribess of Magic"             },
        { "Scribe of Wizardry",         "Scribess of Wizardry"          },
        { "Minor Seer",                 "Minor Seeress"                 },
        { "Seer",                       "Seeress"                       },
        { "Minor Sage",                 "Minor Sage"                    },
        { "Sage",                       "Sage"                          },
        { "Minor Illusionist",          "Minor Illusionist"             },
        { "Illusionist",                "Illusionist"                   },
        { "Minor Abjurer",              "Minor Abjuress"                },
        { "Abjurer",                    "Abjuress"                      },
        { "Minor Invoker",              "Minor Invoker"                 },
        { "Invoker",                    "Invoker"                       },
        { "Minor Enchanter",            "Minor Enchantress"             },
        { "Enchanter",                  "Enchantress"                   },
        { "Minor Conjurer",             "Minor Conjuress"               },
        { "Conjurer",                   "Conjuress"                     },
        { "Minor Magician",             "Minor Witch"                   },
        { "Magician",                   "Witch"                         },
        { "Minor Creator",              "Minor Creator"                 },
        { "Creator",                    "Creator"                       },
        { "Minor Savant",               "Minor Savant"                  },
        { "Savant",                     "Savant"                        },
        { "Minor Magus",                "Minor Craftess"                },
        { "Magus",                      "Craftess"                      },
        { "Minor Wizard",               "Minor Wizard"                  },
        { "Wizard",                     "Wizard"                        },
        { "Minor Warlock",              "Minor War Witch"               },
        { "Warlock",                    "War Witch"                     },
        { "Minor Sorcerer",             "Minor Sorceress"               },
        { "Sorcerer",                   "Sorceress"                     },
        { "Elder Sorcerer",             "Elder Sorceress"               },
        { "Elder Sorcerer",             "Elder Sorceress"               },
        { "Grand Sorcerer",             "Grand Sorceress"               },
        { "Grand Sorcerer",             "Grand Sorceress"               },
        { "Great Sorcerer",             "Great Sorceress"               },
        { "Great Sorcerer",             "Great Sorceress"               },
        { "Golem Maker",                "Golem Maker"                   },
        { "Golem Maker",                "Golem Maker"                   },
        { "Greater Golem Maker",        "Greater Golem Maker"           },
        { "Greater Golem Maker",        "Greater Golem Maker"           },
        { "Maker of Stones",            "Maker of Stones",              },
        { "Maker of Stones",            "Maker of Stones",              },
        { "Maker of Potions",           "Maker of Potions",             },
        { "Maker of Potions",           "Maker of Potions",             },
        { "Maker of Scrolls",           "Maker of Scrolls",             },
        { "Maker of Scrolls",           "Maker of Scrolls",             },
        { "Maker of Wands",             "Maker of Wands",               },
        { "Maker of Wands",             "Maker of Wands",               },
        { "Maker of Staves",            "Maker of Staves",              },
        { "Maker of Staves",            "Maker of Staves",              },
        { "Demon Summoner",             "Demon Summoner"                },
        { "Demon Summoner",             "Demon Summoner"                },
        { "Greater Demon Summoner",     "Greater Demon Summoner"        },
        { "Greater Demon Summoner",     "Greater Demon Summoner"        },
        { "Dragon Charmer",             "Dragon Charmer"                },
        { "Dragon Charmer",             "Dragon Charmer"                },
        { "Greater Dragon Charmer",     "Greater Dragon Charmer"        },
        { "Greater Dragon Charmer",     "Greater Dragon Charmer"        },
        { "Master of all Magic",        "Master of all Magic"           },
        { "Master of all Magic",        "Master of all Magic"           },
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Master Wizard",		"Master Wizard"			},
        { "Wizard Hero",		"Wizard Heroine"		},
        { "Squire of Magic",            "Squire of Magic"               },
        { "Knight of Magic",            "Dame of Magic"			},
        { "Demigod of Magic",           "Demigoddess of Magic"          },
        { "Immortal of Magic",          "Immortal of Magic"             },
        { "God of Magic",               "Goddess of Magic"              },
        { "Deity of Magic",             "Deity of Magic"                },
        { "Supremity of Magic",         "Supremity of Magic"            },
        { "Creator",                    "Creator"                       },
        { "Supreme Deity",              "Supreme Deity"                 }
    },

/*	Priest	*/

    {
	{ "Man",			"Woman"				},

	{ "Believer",			"Believer"			},
	{ "Believer",			"Believer"			},
	{ "Attendant",			"Attendant"			},
	{ "Attendant",			"Attendant"			},
	{ "Acolyte",			"Acolyte"			},
	{ "Acolyte",			"Acolyte"			},
	{ "Novice",			"Novice"			},
	{ "Novice",			"Novice"			},
	{ "Missionary",			"Missionary"			},
	{ "Missionary",			"Missionary"			},
	{ "Adept",			"Adept"				},
	{ "Adept",			"Adept"				},
	{ "Deacon",			"Deaconess"			},
	{ "Deacon",			"Deaconess"			},
	{ "Vicar",			"Vicaress"			},
	{ "Vicar",			"Vicaress"			},
	{ "Priest",			"Priestess"			},
	{ "Priest",			"Priestess"			},
	{ "Minister",			"Lady Minister"			},
	{ "Minister",			"Lady Minister"			},
	{ "Canon",			"Canon"				},
	{ "Canon",			"Canon"				},
	{ "Levite",			"Levitess"			},
	{ "Levite",			"Levitess"			},
	{ "Curate",			"Curess"			},
	{ "Curate",			"Curess"			},
	{ "Monk",			"Nun"				},
	{ "Monk",			"Nun"				},
	{ "Healer",			"Healess"			},
	{ "Healer",			"Healess"			},
	{ "Chaplain",			"Chaplain"			},
	{ "Chaplain",			"Chaplain"			},
	{ "Expositor",			"Expositress"			},
	{ "Expositor",			"Expositress"			},
	{ "Bishop",			"Bishop"			},
	{ "Bishop",			"Bishop"			},
	{ "Arch Bishop",		"Arch Lady of the Church"	},
	{ "Arch Bishop",		"Arch Lady of the Church"	},
	{ "Patriarch",			"Matriarch"			},
	{ "Patriarch",			"Matriarch"			},
	{ "Elder Patriarch",		"Elder Matriarch"		},
	{ "Elder Patriarch",		"Elder Matriarch"		},
	{ "Grand Patriarch",		"Grand Matriarch"		},
	{ "Grand Patriarch",		"Grand Matriarch"		},
	{ "Great Patriarch",		"Great Matriarch"		},
	{ "Great Patriarch",		"Great Matriarch"		},
	{ "Demon Killer",		"Demon Killer"			},
	{ "Demon Killer",		"Demon Killer"			},
	{ "Greater Demon Killer",	"Greater Demon Killer"		},
	{ "Greater Demon Killer",	"Greater Demon Killer"		},
	{ "Cardinal of the Sea",	"Cardinal of the Sea"		},
	{ "Cardinal of the Sea",	"Cardinal of the Sea"		},
	{ "Cardinal of the Earth",	"Cardinal of the Earth"		},
	{ "Cardinal of the Earth",	"Cardinal of the Earth"		},
	{ "Cardinal of the Air",	"Cardinal of the Air"		},
	{ "Cardinal of the Air",	"Cardinal of the Air"		},
	{ "Cardinal of the Ether",	"Cardinal of the Ether"		},
	{ "Cardinal of the Ether",	"Cardinal of the Ether"		},
	{ "Cardinal of the Heavens",	"Cardinal of the Heavens"	},
	{ "Cardinal of the Heavens",	"Cardinal of the Heavens"	},
	{ "Avatar of an Immortal",	"Avatar of an Immortal"		},
	{ "Avatar of an Immortal",	"Avatar of an Immortal"		},
	{ "Avatar of a Deity",		"Avatar of a Deity"		},
	{ "Avatar of a Deity",		"Avatar of a Deity"		},
	{ "Avatar of a Supremity",	"Avatar of a Supremity"		},
	{ "Avatar of a Supremity",	"Avatar of a Supremity"		},
	{ "Avatar of an Implementor",	"Avatar of an Implementor"	},
	{ "Avatar of an Implementor",	"Avatar of an Implementor"	},
	{ "Master of all Divinity",	"Mistress of all Divinity"	},
	{ "Master of all Divinity",	"Mistress of all Divinity"	},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Master Priest",		"Master Priestess"		},
	{ "Divine Hero",		"Divine Heroine"		},
	{ "Divine Squire",		"Divine Squire"			},
	{ "Divine Knight",		"Divine Dame"			},
	{ "Demigod",			"Demigoddess",			},
	{ "Immortal",			"Immortal"			},
	{ "God",			"Goddess"			},
	{ "Deity",			"Deity"				},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",                    "Creator"                       },
	{ "Supreme Deity",		"Supreme Deity"			}
    },

/*	Mercenary	*/

    {
	{ "Man",			"Woman"				},

	{ "Pilferer",			"Pilferess"			},
	{ "Pilferer",			"Pilferess"			},
	{ "Footpad",			"Footpad"			},
	{ "Footpad",			"Footpad"			},
	{ "Filcher",			"Filcheress"			},
	{ "Filcher",			"Filcheress"			},
	{ "Pick-Pocket",		"Pick-Pocket"			},
	{ "Pick-Pocket",		"Pick-Pocket"			},
	{ "Sneak",			"Sneak"				},
	{ "Sneak",			"Sneak"				},
	{ "Pincher",			"Pincheress"			},
	{ "Pincher",			"Pincheress"			},
	{ "Cut-Purse",			"Cut-Purse"			},
	{ "Cut-Purse",			"Cut-Purse"			},
	{ "Snatcher",			"Snatcheress"			},
	{ "Snatcher",			"Snatcheress"			},
	{ "Sharper",			"Sharpress"			},
	{ "Sharper",			"Sharpress"			},
	{ "Rogue",			"Rogue"				},
	{ "Rogue",			"Rogue"				},
	{ "Robber",			"Robber"			},
	{ "Robber",			"Robber"			},
	{ "Magsman",			"Magswoman"			},
	{ "Magsman",			"Magswoman"			},
	{ "Highwayman",			"Highwaywoman"			},
	{ "Highwayman",			"Highwaywoman"			},
	{ "Burglar",			"Burglaress"			},
	{ "Burglar",			"Burglaress"			},
	{ "Thief",			"Thief"				},
	{ "Thief",			"Thief"				},
	{ "Knifer",			"Knifer"			},
	{ "Knifer",			"Knifer"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "Killer",			"Murderess"			},
	{ "Killer",			"Murderess"			},
	{ "Brigand",			"Brigand"			},
	{ "Brigand",			"Brigand"			},
	{ "Cut-Throat",			"Cut-Throat"			},
	{ "Cut-Throat",			"Cut-Throat"			},
	{ "Spy",			"Spy"				},
	{ "Spy",			"Spy"				},
	{ "Grand Spy",			"Grand Spy"			},
	{ "Grand Spy",			"Grand Spy"			},
	{ "Master Spy",			"Master Spy"			},
	{ "Master Spy",			"Master Spy"			},
	{ "Assassin",			"Assassin"			},
	{ "Assassin",			"Assassin"			},
	{ "Greater Assassin",		"Greater Assassin"		},
	{ "Greater Assassin",		"Greater Assassin"		},
	{ "Master of Vision",		"Mistress of Vision"		},
	{ "Master of Vision",		"Mistress of Vision"		},
	{ "Master of Hearing",		"Mistress of Hearing"		},
	{ "Master of Hearing",		"Mistress of Hearing"		},
	{ "Master of Smell",		"Mistress of Smell"		},
	{ "Master of Smell",		"Mistress of Smell"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Touch",		"Mistress of Touch"		},
	{ "Master of Touch",		"Mistress of Touch"		},
	{ "Crime Lord",			"Crime Mistress"		},
	{ "Crime Lord",			"Crime Mistress"		},
	{ "Infamous Crime Lord",	"Infamous Crime Mistress"	},
	{ "Infamous Crime Lord",	"Infamous Crime Mistress"	},
	{ "Greater Crime Lord",		"Greater Crime Mistress"	},
	{ "Greater Crime Lord",		"Greater Crime Mistress"	},
	{ "Master Crime Lord",		"Master Crime Mistress"		},
	{ "Master Crime Lord",		"Master Crime Mistress"		},
	{ "Godfather",			"Godmother"			},
	{ "Godfather",			"Godmother"			},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
        { "Master Mercenary",		"Master Mercenary"		},
	{ "Assassin Hero",		"Assassin Heroine"		},
	{ "Squire of Death",		"Squire of Death",		},
	{ "Knight of Death",		"Dame of Death"			},
	{ "Demigod of Assassins",	"Demigoddess of Assassins"	},
	{ "Immortal Assasin",		"Immortal Assassin"		},
	{ "God of Assassins",		"God of Assassins",		},
	{ "Deity of Assassins",		"Deity of Assassins"		},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",                    "Creator"                       },
	{ "Supreme Deity",		"Supreme Deity"			}
    },

/*	Gladiator	*/

    {
	{ "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},
	{ "Soldier",			"Soldier"			},
	{ "Warrior",			"Warrior"			},
	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},
	{ "Combatant",			"Combatess"			},
	{ "Hero",			"Heroine"			},
	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},
	{ "Swordmaster",		"Swordmistress"			},
	{ "Lieutenant",			"Lieutenant"			},
	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},
	{ "Knight",			"Lady Knight"			},
	{ "Grand Knight",		"Grand Knight"			},
	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Strider",			"Strider"			},
	{ "Strider",			"Strider"			},
	{ "Grand Strider",		"Grand Strider"			},
	{ "Grand Strider",		"Grand Strider"			},
	{ "Demon Slayer",		"Demon Slayer"			},
	{ "Demon Slayer",		"Demon Slayer"			},
	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},
	{ "Overlord",			"Overlord"			},
	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Meteors",		"Baroness of Meteors"		},
	{ "Baron of Meteors",		"Baroness of Meteors"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Centurian Hero",		"Centurian Heroine"		},
	{ "Squire of War",		"Squire of War"			},
	{ "Knight of War",		"Dame of War"			},
	{ "Demigod of War",		"Demigoddess of War"		},
	{ "Immortal Warlord",		"Immortal Warlord"		},
	{ "God of War",			"God of War"			},
	{ "Deity of War",		"Deity of War"			},
	{ "Supreme Master of War",	"Supreme Mistress of War"	},
        { "Creator",                    "Creator"                       },
	{ "Supreme Deity",		"Supreme Deity"			}
    },

/*	Strider	*/

    {
	{ "Man",			"Woman"				},

	{ "Apprentice of the Hunt",	"Apprentice of the Hunt"	},
	{ "Student of the Hunt",	"Student of the Hunt"		},
	{ "Student of the Hunt",	"Student of the Hunt"		},
	{ "Runner",			"Runner"			},
	{ "Runner",			"Runner"			},
	{ "Strider",			"Strider"			},
	{ "Strider",			"Strider"			},
	{ "Scout",			"Scout"				},
	{ "Scout",			"Scout"				},
	{ "Master Scout",		"Master Scout"			},
	{ "Master Scout",		"Master Scout"			},
	{ "Explorer",			"Explorer"			},
	{ "Explorer",			"Explorer"			},
	{ "Tracker",			"Tracker"			},
	{ "Tracker",			"Tracker"			},
	{ "Survivalist",		"Survivalist"			},
	{ "Survivalist",		"Survivalist"			},
	{ "Watcher",			"Watcher"			},
	{ "Watcher",			"Watcher"			},
	{ "Hunter",			"Hunter"			},
	{ "Hunter",			"Hunter"			},
	{ "Woodsman",			"Woodswoman"			},
	{ "Woodsman",			"Woodswoman"			},
	{ "Guide",			"Guide"				},
	{ "Guide",			"Guide"				},
	{ "Pathfinder",			"Pathfinder"			},
	{ "Pathfinder",			"Pathfinder"			},
	{ "Stalker",			"Stalker"			},
	{ "Stalker",			"Stalker"			},
	{ "Predator",			"Predator"			},
	{ "Predator",			"Predator"			},
	{ "Deerhunter",			"Deerhunter"			},
	{ "Deerhunter",			"Deerhunter"			},
	{ "Elkhunter",			"Elkhunter"			},
	{ "Elkhunter",			"Elkhunter"			},
	{ "Boarhunter",			"Boarhunter"			},
	{ "Boarhunter",			"Boarhunter"			},
	{ "Bearhunter",			"Bearhunter"			},
	{ "Bearhunter",			"Bearhunter"			},
	{ "Falconer",			"Falconer"			},
	{ "Falconer",			"Falconer"			},
	{ "Hawker",			"Hawker"			},
	{ "Hawker",			"Hawker"			},
	{ "Manhunter",			"Manhunter"			},
	{ "Manhunter",			"Manhunter"			},
	{ "Gianthunter",		"Gianthunter"			},
	{ "Gianthunter",		"Gianthunter"			},
	{ "Adept of the Hunt",		"Adept of the Hunt"		},
	{ "Adept of the Hunt",		"Adept of the Hunt"		},
	{ "Lord of the Hunt",		"Lady of the Hunt"		},
	{ "Lord of the Hunt",		"Lady of the Hunt"		},
	{ "Master of the Hunt",		"Mistress of the Hunt"		},
	{ "Master of the Hunt",		"Mistress of the Hunt"		},
	{ "Fox",			"Fox"				},
	{ "Coyote",			"Coyote"			},
	{ "Lynx",			"Lynx"				},
	{ "Wolf",			"Wolf"				},
	{ "Panther",			"Panther"			},
	{ "Cougar",			"Cougar"			},
	{ "Jaguar",			"Jaguar"			},
	{ "Tiger",			"Tiger"				},
	{ "Lion",			"Lion"				},
	{ "Bear",			"Bear"				},
	{ "Beastmaster",		"Beastmaster"			},
	{ "Beastmaster",		"Beastmaster"			},
	{ "Overlord of Beasts",		"Overlord of Beasts"		},
	{ "Overlord of Beasts",		"Overlord of Beasts"		},
	{ "Dragonhunter",		"Dragonhunter"			},
	{ "Dragonhunter",		"Dragonhunter"			},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Master Strider",		"Master Strider"		},
	{ "Strider Hero",		"Strider Heroine"		},
	{ "Squire of the Hunt",		"Squire of the Hunt"		},
	{ "Knight of the Hunt",		"Dame of the Hunt"		},
	{ "Demigod of Beasts",		"Demigoddess of Beasts"		},
	{ "Immortal Hunter",		"Immortal Huntress"		},
	{ "God of Beasts",		"Goddess of Beasts"		},
	{ "Deity of Beasts",		"Deity of Beasts"		},
	{ "Supremity of Beasts",	"Supremity of Beasts"		},
	{ "Creator",			"Creator"			},
	{ "Supreme Deity",		"Supreme Deity"			},
    },

/*	Sage	*/

    {
	{ "Man",			"Woman"				},

	{ "Apprentice of Nature",	"Apprentice of Nature"		},
	{ "Student of Nature",		"Student of Nature"		},
	{ "Student of Nature",		"Student of Nature"		},
	{ "Scholar of Nature",		"Scholar of Nature"		},
	{ "Scholar of Nature",		"Scholar of Nature"		},
	{ "Neophyte",			"Neophyte"			},
	{ "Neophyte",			"Neophyte"			},
	{ "Naturalist",			"Naturalist"			},
	{ "Naturalist",			"Naturalist"			},
	{ "Forester",			"Forestress"			},
	{ "Forester",			"Forestress"			},
	{ "Student of Earth",		"Student of Earth"		},
	{ "Student of Earth",		"Student of Earth"		},
	{ "Student of Air",		"Student of Air"		},
	{ "Student of Air",		"Student of Air"		},
	{ "Student of Water",		"Student of Water"		},
	{ "Student of Water",		"Student of Water"		},
	{ "Student of Fire",		"Student of Fire"		},
	{ "Student of Fire",		"Student of Fire"		},
	{ "Student of Life",		"Student of Life"		},
	{ "Student of Life",		"Student of Life"		},
	{ "Student of The Elements",	"Student of The Elements"	},
	{ "Student of The Elements",	"Student of The Elements"	},
	{ "Herbalist",			"Herbalist"			},
	{ "Herbalist",			"Herbalist"			},
	{ "Philosopher",		"Philosopher"			},
	{ "Philosopher",		"Philosopher"			},
	{ "Sage",			"Sage"				},
	{ "Sage",			"Sage"				},
	{ "Prophet",			"Prophetess"			},
	{ "Prophet",			"Prophetess"			},
	{ "Adept of Earth",		"Adept of Earth"		},
	{ "Adept of Earth",		"Adept of Earth"		},
	{ "Adept of Air",		"Adept of Air"			},
	{ "Adept of Air",		"Adept of Air"			},
	{ "Adept of Water",		"Adept of Water"		},
	{ "Adept of Water",		"Adept of Water"		},
	{ "Adept of Fire",		"Adept of Fire"			},
	{ "Adept of Fire",		"Adept of Fire"			},
	{ "Adept of Life",		"Adept of Life"			},
	{ "Adept of Life",		"Adept of Life"			},
	{ "Adept of The Elements",	"Adept of The Elements"		},
	{ "Adept of The Elements",	"Adept of The Elements"		},
	{ "Druid of Earth",		"Druidess of Earth"		},
	{ "Druid of Earth",		"Druidess of Earth"		},
	{ "Druid of Air",		"Druidess of Air"		},
	{ "Druid of Air",		"Druidess of Air"		},
	{ "Druid of Water",		"Druidess of Water"		},
	{ "Druid of Water",		"Druidess of Water"		},
	{ "Druid of Fire",		"Druidess of Fire"		},
	{ "Druid of Fire",		"Druidess of Fire"		},
	{ "Druid of Life",		"Druidess of Life"		},
	{ "Druid of Life",		"Druidess of Life"		},
	{ "Druid of The Elements",	"Druidess of The Elements"	},
	{ "Druid of The Elements",	"Druidess of The Elements"	},
	{ "Sage of Earth",		"Sage of Earth"			},
	{ "Sage of Earth",		"Sage of Earth"			},
	{ "Sage of Air",		"Sage of Air"			},
	{ "Sage of Air",		"Sage of Air"			},
	{ "Sage of Water",		"Sage of Water"			},
	{ "Sage of Water",		"Sage of Water"			},
	{ "Sage of Fire",		"Sage of Fire"			},
	{ "Sage of Fire",		"Sage of Fire"			},
	{ "Sage of Life",		"Sage of Life"			},
	{ "Sage of Life",		"Sage of Life"			},
	{ "Sage of The Elements",	"Sage of The Elements"		},
	{ "Sage of The Elements",	"Sage of The Elements"		},
	{ "Master of Earth",		"Mistress of Earth"		},
	{ "Master of Earth",		"Mistress of Earth"		},
	{ "Master of Air",		"Mistress of Air"		},
	{ "Master of Air",		"Mistress of Air"		},
	{ "Master of Water",		"Mistress of Water"		},
	{ "Master of Water",		"Mistress of Water"		},
	{ "Master of Fire",		"Mistress of Fire"		},
	{ "Master of Fire",		"Mistress of Fire"		},
	{ "Master of Life",		"Mistress of Life"		},
	{ "Master of Life",		"Mistress of Life"		},
	{ "Master of The Elements",	"Mistress of The Elements"	},
	{ "Master of The Elements",	"Mistress of The Elements"	},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Master Sage",		"Master Sage"			},
	{ "Sage Hero",			"Sage Heroine"			},
	{ "Squire of The Elements",	"Squire of The Elements"	},
	{ "Knight of The Elements",	"Dame of The Elements"		},
	{ "Demigod of Nature",		"Demigoddess of Nature"		},
	{ "Immortal Sage",		"Immortal Sage"			},
	{ "God of The Elements",	"Goddess of The Elements"	},
	{ "Deity of The Elements",	"Deity of The Elements"		},
	{ "Supremity of The Elements",	"Supremity of The Elements"	},
	{ "Creator",			"Creator"			},
	{ "Supreme Diety",		"Supreme Diety"			},
    },

/*	Lich	*/
    {
	{ "Man",			"Woman"				},

	{ "Blood Student",		"Blood Student"			},
	{ "Scholar of Blood",		"Scholar of Blood"		},
	{ "Scholar of Blood",		"Scholar of Blood"		},
	{ "Student of the Night",	"Student of the Night"		},
	{ "Student of the Night",	"Student of the Night"		},
	{ "Student of Death",		"Student of Death"		},
	{ "Student of Death",		"Student of Death"		},
	{ "Lesser Imp",			"Lesser Imp"			},
	{ "Greater Imp",		"Greater Imp"			},
	{ "Illusionist",		"Illusionist"			},
	{ "Illusionist",		"Illusionist"			},
	{ "Seducer",			"Seductress"			},
	{ "Seducer",			"Seductress"			},
	{ "Stalker",			"Stalker"			},
	{ "Stalker",			"Stalker"			},
	{ "Stalker of Blood",		"Stalker of Blood"		},
	{ "Stalker of Blood",		"Stalker of Blood"		},
	{ "Stalker of Night",		"Stalker of Night"		},
	{ "Stalker of Night",		"Stalker of Night"		},
	{ "Stalker of Death",		"Stalker of Death"		},
	{ "Stalker of Death",		"Stalker of Death"		},
	{ "Shadow",			"Shadow"			},
	{ "Shadow",			"Shadow"			},
	{ "Shadow of Blood",		"Shadow of Blood"		},
	{ "Shadow of Blood",		"Shadow of Blood"		},
	{ "Shadow of Night",		"Shadow of Night"		},
	{ "Shadow of Night",		"Shadow of Night"		},
	{ "Shadow of Death",		"Shadow of Death"		},
	{ "Shadow of Death",		"Shadow of Death"		},
	{ "Killer",			"Killer"			},
	{ "Killer",			"Killer"			},
	{ "Murderer",			"Murderer"			},
	{ "Murderer",			"Murderer"			},
	{ "Incubus",			"Succubus"			},
	{ "Incubus",			"Succubus"			},
	{ "Nightstalker",		"Nightstalker"			},
	{ "Nightstalker",		"Nightstalker"			},
	{ "Revenant",			"Revenant"			},
	{ "Revenant",			"Revenant"			},
	{ "Lesser Vrolok",		"Lesser Vrolok"			},
	{ "Vrolok",			"Vrolok"			},
	{ "Greater Vrolok",		"Greater Vrolok"		},
	{ "Demon Servant",		"Demon Servant"			},
	{ "Demon Servant",		"Demon Servant"			},
	{ "Adept of Blood",		"Adept of Blood"		},
	{ "Adept of Blood",		"Adept of Blood"		},
	{ "Adept of Night",		"Adept of Night"		},
	{ "Adept of Night",		"Adept of Night"		},
	{ "Adept of Death",		"Adept of Death"		},
	{ "Adept of Death",		"Adept of Death"		},
	{ "Demon Seeker",		"Demon Seeker"			},
	{ "Demon Seeker",		"Demon Seeker"			},
	{ "Lord of Blood",		"Lady of Blood"			},
	{ "Lord of Blood",		"Lady of Blood"			},
	{ "Lord of Night",		"Lady of Night"			},
	{ "Lord of Night",		"Lady of Night"			},
	{ "Lord of Death",		"Lady of Death"			},
	{ "Lord of Death",		"Lady of Death"			},
	{ "Demon Knight",		"Demon Dame"			},
	{ "Demon Knight",		"Demon Dame"			},
	{ "Master of Blood",		"Mistress of Blood"		},
	{ "Master of Blood",		"Mistress of Blood"		},
	{ "Master of Night",		"Mistress of Night"		},
	{ "Master of Night",		"Mistress of Night"		},
	{ "Master of Death",		"Mistress of Death"		},
	{ "Master of Death",		"Mistress of Death"		},
	{ "Lord of Demons",		"Lady of Demons"		},
	{ "Lord of Demons",		"Lady of Demons"		},
	{ "Baron of Demons",		"Baroness of Demons"		},
	{ "Baron of Demons",		"Baroness of Demons"		},
	{ "Master of Demons",		"Mistress of Demons"		},
	{ "Master of Demons",		"Mistress of Demons"		},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Master Lich",		"Master Lich"			},
	{ "Lich Hero",			"Lich Heroine"			},
	{ "Squire of Blood",		"Squire of Blood"		},
	{ "Knight of Blood",		"Dame of Blood"			},
	{ "Demigod of the Night",	"Demigoddess of the Night"	},
	{ "Immortal Undead",		"Immortal Undead"		},
	{ "God of Death",		"Goddess of Death"		},
	{ "Deity of Death",		"Deity of Death"		},
	{ "Supremity of Death",		"Supremity of Death"		},
	{ "Creator",			"Creator"			},
	{ "Supreme Diety",		"Supreme Deity"			},
    }
};



/*
 * Attribute bonus tables.
 */
const	struct	str_app_type	str_app		[MAX_STAT]		=
{
    { -10, -4,   0,  0 },  /* 0  */
    { -10, -4,   3,  1 },  /* 1  */
    { -8, -2,   3,  2 },
    { -8, -1,  10,  3 },  /* 3  */
    { -7, -1,  25,  4 },
    { -7, -1,  55,  5 },  /* 5  */
    { -6,  0,  80,  6 },
    { -5,  0,  90,  7 },
    { -4,  0, 100,  8 },
    { -3,  0, 100,  9 },
    { -2,  0, 115, 10 }, /* 10  */
    { -1,  0, 115, 11 },
    {  0,  0, 130, 12 },
    {  0,  1, 130, 13 }, /* 13  */
    {  0,  1, 140, 14 },
    {  1,  2, 150, 15 }, /* 15  */
    {  1,  2, 165, 16 },
    {  2,  3, 180, 22 },
    {  2,  5, 200, 25 }, /* 18  */
    {  3,  7, 225, 30 },
    {  5, 10, 250, 35 }, /* 20  */
    { 10, 15, 300, 40 },
    { 15, 19, 350, 45 },
    { 20, 23, 400, 50 },
    { 25, 25, 450, 55 },
    { 35, 30, 500, 60 }, /* 25   */
    { 40, 35, 550, 65 },
    { 45, 40, 600, 70 },
    { 50, 45, 650, 75 },
    { 55, 50, 700, 80 },
    { 60, 55, 999, 99 }  /* 30   */
};

/*
 * Attribute bonus tables.
 */
const	struct	str_app_type	str_app_new	[MAX_STAT_NEW]		=
{
    { -10, -4,   0,  0 },  /* 0  */
    { -10, -4,   3,  1 },  /* 1  */
    { -8, -2,   3,  2 },
    { -8, -1,  10,  3 },  /* 3  */
    { -7, -1,  25,  4 },
    { -7, -1,  55,  5 },  /* 5  */
    { -6,  0,  80,  6 },
    { -5,  0,  90,  7 },
    { -4,  0, 100,  8 },
    { -3,  0, 100,  9 },
    { -2,  0, 115, 10 }, /* 10  */
    { -1,  0, 115, 11 },
    {  0,  0, 130, 12 },
    {  0,  1, 130, 13 }, /* 13  */
    {  0,  1, 140, 14 },
    {  1,  2, 150, 15 }, /* 15  */
    {  1,  2, 165, 16 },
    {  2,  3, 180, 22 },
    {  2,  5, 200, 25 }, /* 18  */
    {  3,  7, 225, 30 },
    {  5, 10, 250, 35 }, /* 20  */
    { 10, 15, 300, 40 },
    { 15, 19, 350, 45 },
    { 20, 23, 400, 50 },
    { 25, 25, 450, 55 },
    { 35, 30, 500, 60 }, /* 25   */
    { 40, 35, 550, 65 },
    { 45, 40, 600, 70 },
    { 50, 45, 650, 75 },
    { 55, 50, 700, 80 },

    { 60, 55, 999, 99 },  /* 30   */

    { 41, 36, 520, 75 },  /* 31   */
    { 42, 37, 540, 76 },  /* 32   */
    { 43, 38, 560, 77 },  /* 33   */
    { 44, 39, 580, 78 },  /* 34   */

    { 45, 40, 600, 79 },  /* 35   */
    { 46, 41, 620, 80 },  /* 36   */
    { 47, 42, 640, 81 },  /* 37   */
    { 48, 43, 660, 82 },  /* 38   */
    { 49, 44, 680, 83 },  /* 39   */

    { 50, 45, 700, 84 },  /* 40   */
    { 52, 47, 740, 86 },  /* 41   */
    { 54, 49, 780, 88 },  /* 42   */
    { 56, 51, 820, 90 },  /* 43   */
    { 58, 53, 860, 92 },  /* 44   */

    { 60, 55, 900, 94 },  /* 45   */
    { 61, 56, 920, 95 },  /* 46   */
    { 62, 57, 940, 96 },  /* 47   */
    { 63, 58, 960, 97 },  /* 48   */
    { 64, 59, 980, 98 },  /* 49   */
    { 65, 60, 999, 99 }  /* 50   */
};



const	struct	int_app_type	int_app		[MAX_STAT]	=
{
    {  3 },	/*  0 */
    {  5 },	/*  1 */
    {  7 },
    {  8 },	/*  3 */
    {  9 },
    { 10 },	/*  5 */
    { 11 },
    { 12 },
    { 13 },
    { 15 },
    { 17 },	/* 10 */
    { 19 },
    { 22 },
    { 25 },
    { 28 },
    { 31 },	/* 15 */
    { 34 },
    { 37 },
    { 40 },	/* 18 */
    { 44 },
    { 49 },	/* 20 */
    { 55 },
    { 60 },
    { 65 },
    { 70 },
    { 75 },	/* 25 */
    { 80 },
    { 85 },
    { 90 },
    { 95 },
    { 99 }      /* 30 */

};



const	struct	wis_app_type	wis_app		[MAX_STAT]	=
{
    { 0 },	/*  0 */
    { 0 },	/*  1 */
    { 0 },
    { 0 },	/*  3 */
    { 0 },
    { 1 },	/*  5 */
    { 1 },
    { 1 },
    { 1 },
    { 1 },
    { 2 },	/* 10 */
    { 2 },
    { 2 },
    { 2 },
    { 2 },
    { 3 },	/* 15 */
    { 3 },
    { 4 },
    { 5 },      /* 18 */
    { 5 },
    { 6 },      /* 20 */
    { 6 },
    { 7 },
    { 7 },
    { 8 },
    { 8 },      /* 25 */
    { 9 },
   { 10 },
   { 11 },
   { 12 },
   { 13 }
};



const	struct	dex_app_type	dex_app		[MAX_STAT]	=
{
    {   60 },   /* 0 */
    {   50 },   /* 1 */
    {   50 },
    {   40 },
    {   30 },
    {   20 },   /* 5 */
    {   10 },
    {    0 },
    {    0 },
    {    0 },
    {    0 },   /* 10 */
    {    0 },
    {    0 },
    {    0 },
    {    0 },
    { - 10 },   /* 15 */
    { - 15 },
    { - 20 },
    { - 30 },
    { - 40 },
    { - 50 },   /* 20 */
    { - 60 },
    { - 75 },
    { - 90 },
    { -105 },
    { -120 },   /* 25 */
    { -140 },
    { -160 },
    { -185 },
    { -210 },
    { -240 }    /* 30 */
};



const	struct	con_app_type	con_app		[MAX_STAT]	=
{
    { -4, 20 },   /*  0 */
    { -3, 25 },   /*  1 */
    { -2, 30 },
    { -2, 35 },	  /*  3 */
    { -1, 40 },
    { -1, 45 },   /*  5 */
    { -1, 50 },
    {  0, 55 },
    {  0, 60 },
    {  0, 65 },
    {  0, 70 },   /* 10 */
    {  0, 75 },
    {  0, 80 },
    {  0, 85 },
    {  0, 88 },
    {  1, 90 },   /* 15 */
    {  2, 95 },
    {  2, 97 },
    {  3, 99 },   /* 18 */
    {  3, 99 },
    {  4, 99 },   /* 20 */
    {  4, 99 },
    {  5, 99 },
    {  6, 99 },
    {  7, 99 },
    {  8, 99 },   /* 25 */
    { 10, 100 },
    { 12, 100 },
    { 14, 100 },
    { 17, 100 },
    { 20, 100 }   /*30*/

};



/*
 * Liquid properties.
 * Used in world.obj.
 * Be sure MAX_LIQUID in merc.h matches
 * the number of entries below.
 */
const	struct	liq_type	liq_table	[]	=
{
/*    name			color	proof, full, thirst, food, ssize */
    { "water",			"clear",	{   0, 1, 10, 0, 16 }	},
    { "beer",			"amber",	{  12, 1,  8, 1, 12 }	},
    { "red wine",		"burgundy",	{  30, 1,  8, 1,  5 }	},
    { "ale",			"brown",	{  15, 1,  8, 1, 12 }	},
    { "dark ale",		"dark",		{  16, 1,  8, 1, 12 }	},

    { "whisky",			"golden",	{ 120, 1,  5, 0,  2 }	},
    { "lemonade",		"pink",		{   0, 1,  9, 2, 12 }	},
    { "firebreather",		"boiling",	{ 190, 0,  4, 0,  2 }	},
    { "local specialty",	"clear",	{ 151, 1,  3, 0,  2 }	},
    { "slime mold juice",	"green",	{   0, 2, -8, 1,  2 }	},

    { "milk",			"white",	{   0, 2,  9, 3, 12 }	},
    { "tea",			"tan",		{   0, 1,  8, 0,  6 }	},
    { "coffee",			"black",	{   0, 1,  8, 0,  6 }	},
    { "blood",			"red",		{   0, 2,  0, 2,  6 }	},
    { "salt water",		"clear",	{   0, 1, -2, 0,  1 }	},

    { "coke",			"brown",	{   0, 2,  9, 2, 12 }	},
    { "root beer",		"brown",	{   0, 2,  9, 2, 12 }   },
    { "elvish wine",		"green",	{  35, 2,  8, 1,  5 }   },
    { "white wine",		"golden",	{  28, 1,  8, 1,  5 }   },
    { "champagne",		"golden",	{  32, 1,  8, 1,  5 }   },

    { "mead",			"honey-colored",{  34, 2,  8, 2, 12 }   },
    { "rose wine",		"pink",		{  26, 1,  8, 1,  5 }	},
    { "benedictine wine",	"burgundy",	{  40, 1,  8, 1,  5 }   },
    { "vodka",			"clear",	{ 130, 1,  5, 0,  2 }   },
    { "cranberry juice",	"red",		{   0, 1,  9, 2, 12 }	},

    { "orange juice",		"orange",	{   0, 2,  9, 3, 12 }   },
    { "absinthe",		"green",	{ 200, 1,  4, 0,  2 }	},
    { "brandy",			"golden",	{  80, 1,  5, 0,  4 }	},
    { "aquavit",		"clear",	{ 140, 1,  5, 0,  2 }	},
    { "schnapps",		"clear",	{  90, 1,  5, 0,  2 }   },

    { "icewine",		"purple",	{  50, 2,  6, 1,  5 }	},
    { "amontillado",		"burgundy",	{  35, 2,  8, 1,  5 }	},
    { "sherry",			"red",		{  38, 2,  7, 1,  5 }   },
    { "framboise",		"red",		{  50, 1,  7, 1,  5 }   },
    { "rum",			"amber",	{ 151, 1,  4, 0,  2 }	},

    { "cordial",		"clear",	{ 100, 1,  5, 0,  2 }   },
    { "dr pepper",		"brown",	{   0, 2,  9, 2, 12 }	},
    { NULL,			NULL,		{   0, 0,  0, 0,  0 }	}
};



/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)	n

struct	skill_type	skill_table	[MAX_SKILL]	=
{

/*
 * Magic spells.
 */

/*
    {
	"NAME",
	{ SKILL_LEVELS 1st TIER,
	  SKILL_LEVELS 2nd TIER },
	{ DIFFICULTY 1st TIER,
	  DIFFICULTY 2nd TIER },
	SPELL_FUN,		TARGET,		MIN_POSITION,
	GSN,		SOCKET,	SLOT(#),	MIN_MANA,	BEATS, COOLDOWN,
	"DAMAGE NOUN",	"OFF MESSAGE",	"OFF MESSAGE (OBJ)"
    }

	NAME		Name of Spell/Skill
	SKILL_LEVELS	Levels to obtain spell/skill per class
	DIFFICULTY	Spells: multiplier for base mana
			Skills: charge for gaining skill
	SPELL_FUN	Routine to call for spells
	TARGET		Legal targets
	MIN_POSTITION	Position for caster/user
	GSN		gsn for skills and some spells
	SOCKET		Can spell be cast on person from same socket
			(to limit multiplaying)
	SLOT(#)		A unique slot number for spells
	MIN_MANA	Base mana for spells (multiplied by DIFFICULTY)
	BEATS		Waiting time after use
	DAMAGE NOUN	Damage Message
	OFF MESSAGE	Wear off message
	OFF MESSAGE (OBJ)	Wear off message for objects
*/

    {
	"reserved",
	{ 199, 199, 199, 199, 199, 199, 199,
	  199, 199, 199, 199, 199, 199, 199,
	199, 199, 199,	199, 199, 199,
	199, 199, 199,	199, 199, 199,
	199, 199, 199,	199, 199, 199,
	199, 199, 199 },
	{ 99, 99, 99, 99, 99, 99, 99,
	  99, 99, 99, 99, 99, 99, 99,
	99, 99, 99,		99, 99, 99,
	99, 99, 99,		99, 99, 99,
	99, 99, 99,		99, 99, 99,
	99, 99, 99 },
	0,			TAR_IGNORE,	POS_STANDING,
	NULL,		TRUE,	SLOT( 0),	0,	0,	0,
	"",		"",		"",NULL,NULL
    },

    {
	"acid blast",
	{ 55, 93, 103, 103, 102, 45, 65,
	  45, 83, 103, 103, 102, 35, 55,
	35, 35, 35,		73, 73, 73,
	103, 103, 103,	103, 103, 103,
	102, 102, 102,	25, 25, 25,
	45, 45, 45 },
	{ 1, 1, 0, 0, 2, 1, 1,
	  1, 1, 0, 0, 2, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1 },
	spell_acid_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(70),	20,	12,	0,
	"acid blast",	"!Acid Blast!",		"",NULL,NULL
    },

   {
  	"acid rain",
	{ 103, 103, 103, 103, 103, 75, 103,
          103, 103, 103, 103, 103, 70, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	65, 65, 65,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 5, 0,
	  0, 0, 0, 0, 0, 3, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	2, 2, 2,
	0, 0, 0 },
        spell_acid_rain,    	TAR_CHAR_OFFENSIVE,    POS_FIGHTING,
	NULL,       	TRUE,	SLOT(610),	95,	18,	0,
        "acid rain",	"!Acid Rain!",		"",NULL,NULL
    },

    {
	"animate",
	{ 103, 103, 103, 103, 103, 103, 45,
	  103, 103, 103, 103, 103, 103, 35,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	25, 25, 25 },
	{ 0, 0, 0, 0, 0, 0, 1,
	  0, 0, 0, 0, 0, 0, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	1, 1, 1 },
	spell_animate,		TAR_OBJ_INV,		POS_STANDING,
	NULL,		TRUE,	SLOT(239),	 20,	12,	0,
	"",			"!Animate!",	"",NULL,NULL
    },

    {
	"armor",
	{ 13, 2, 102, 40, 15, 23, 102,
	  3, 1, 102, 30,  5, 13, 102,
	2, 2, 2,		1, 1, 1,
	102, 102, 102,	20, 20, 20,
	3, 3, 3,		3, 3, 3,
	102, 102, 102 },
	{ 1, 1, 0, 2, 2, 1, 0,
	  1, 1, 0, 2, 2, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	2, 2, 2,
	2, 2, 2,	1, 1, 1,
	0, 0, 0 },
	spell_armor,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		FALSE,	SLOT( 1),	 5,	12,	0,
	"",			"You feel less armored.",	"",NULL,NULL
    },

    {
	"garrote",
		{ 13, 2, 102, 40, 15, 23, 102,
	  	3, 1, 102, 30,  5, 13, 102,
	2, 2, 2,		1, 1, 1,
	102, 102, 102,	20, 20, 20,
	3, 3, 3,		3, 3, 3,
	102, 102, 102 },
	{ 1, 1, 0, 2, 2, 1, 0,
	  1, 1, 0, 2, 2, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	2, 2, 2,
	2, 2, 2,	1, 1, 1,
	0, 0, 0 },
	spell_null, TAR_IGNORE, POS_STANDING,
	&gsn_garrote, TRUE, SLOT( 0), 0, 22,	0,
	"garrote", "You can breath again.", "",NULL,NULL
	},

    {
	"bless",
	{ 102, 13, 102, 102, 102, 16, 102,
	  75, 3, 102, 102, 102, 6, 102,
	65, 65, 65,		2, 2, 2,
	102, 102, 102,	102, 102, 102,
	102, 102, 102,	4, 4, 4,
	102, 102, 102 },
	{ 0, 1, 0, 0, 0, 1, 0,
	  1, 1, 0, 0, 0, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0 },
	spell_bless,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	NULL,		FALSE,	SLOT( 3),	 5,	12,	0,
	"",			"You feel less righteous.",
	"$p's holy aura fades.",NULL,NULL
    },

    {
	"blindness",
	{ 24, 15, 103, 103, 65, 14, 34,
	  14, 5, 103, 103, 55, 4, 24,
	4, 4, 4,		3, 3, 3,
	103, 103, 103,	103, 103, 103,
	45, 45, 45,		3, 3, 3,
	14, 14, 14 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_blindness,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_blindness,	TRUE,	SLOT( 4),	 5,	12,	0,
	"",			"You can see again.",	"",NULL,NULL
    },

    {
	"burning hands",
	{ 13, 17, 103, 103, 103, 23, 103,
	  3, 7, 103, 103, 103, 13, 85,
	2, 2, 2,		4, 4, 4,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	5, 5, 5,
	75, 75, 75 },
	{ 1, 1, 0, 0, 0, 1, 0,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_burning_hands,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT( 5),	15,	12,	0,
	"burning hands",	"!Burning Hands!", 	"",NULL,NULL
    },

    {
	"call lightning",
	{ 103, 36, 103, 103, 30, 48, 103,
	  92, 26, 103, 103, 20, 38, 103,
	82, 82, 82,		16, 16, 16,
	103, 103, 103,	103, 103, 103,
	10, 10, 10,		28, 28, 28,
	103, 103, 103 },
	{ 0, 1, 0, 0, 0, 1, 0,
	  1, 1, 0, 0, 0, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0 },
	spell_call_lightning,	TAR_IGNORE,		POS_FIGHTING,
	NULL,		TRUE,	SLOT( 6),	15,	12,	0,
	"lightning bolt",	"!Call Lightning!",	"",NULL,NULL
    },

    {
	"calm",
	{ 95, 32, 102, 102, 90, 46, 59,
	  85, 22, 102, 102, 80, 36, 49,
	75, 75, 75,		12, 12, 12,
	102, 102, 102,	102, 102, 102,
	70, 70, 70,		26, 26, 26,
	39, 39, 39 },
	{ 1, 1, 0, 0, 0, 1, 2,
	  1, 1, 0, 0, 0, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	2, 2, 2 },
	spell_calm,		TAR_IGNORE,		POS_FIGHTING,
	NULL,		TRUE,	SLOT(509),	30,	12,	0,
	"",			"You have lost your peace of mind.",	"",NULL,NULL
    },

    {
	"cancellation",
	{ 51, 51, 102, 102, 51, 46, 102,
	  41, 41, 102, 102, 41, 36, 102,
	31, 31, 31,		31, 31, 31,
	102, 102, 102,	102, 102, 102,
	31, 31, 31,		26, 26, 26,
	102, 102, 102
 },
	{ 1, 1, 0, 0, 0, 1, 0,
	  1, 1, 0, 0, 0, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0 },
	spell_cancellation,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,		FALSE,	SLOT(507),	20,	12,	0,
	""			"!cancellation!",	"",NULL,NULL
    },

    {
	"siphon energy",
	{ 45, 45, 103, 103, 50, 85, 21,
	  35, 35, 103, 103, 45, 75, 11,
	25, 25, 25,		25, 25, 25,
	103, 103, 103,	103, 103, 103,
	40, 40, 40,		65, 65, 65,
	8, 8, 8 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_siphon_energy,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_siphon,		TRUE,	SLOT(63),	175,	12,	0,
	"{yenergy drawing essence{x",		"{mYour link fades away!{x",	"",NULL,NULL
    },

    {
	"siphon life",
	{
          100, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103,  25, 103
        },
	{
            1,   1,   0,   0,   2,   1,  1,
	    1,   1,   0,   0,   2,   1,  1,
	    1,   1,   1,   1,   1,   1,  0,
            0,   0,   0,   0,   0,   2,  2,
            2,   1,   1,   1,	1,   1,  1
        },
	spell_siphon_life,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_siphon,		TRUE,	SLOT(62),	175,	12,	0,
	"{ylife drawing essence{x",		"{mYour link fades away{x",	"",NULL,NULL
    },

    {
	"cause serious",
	{ 20, 23, 103, 103, 103, 53, 11,
	  10, 13, 103, 103, 103, 43, 9,
	6, 6, 6,		8, 8, 8,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	33, 33, 33,
	5, 5, 5 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_cause_serious,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(64),	17,	12,	0,
	"spell",		"!Cause Serious!",	"",NULL,NULL
    },

    {
	"chain lightning",
	{ 66, 103, 103, 103, 103, 86, 103,
	  55, 103, 103, 103, 103, 76, 103,
	44, 44, 44,		103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	66, 66, 66,
	103, 103, 103 },
	{ 1, 0, 0, 0, 0, 1, 1,
	  1, 0, 0, 0, 0, 1, 1,
	1, 1, 1,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_chain_lightning,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
        NULL,           TRUE,   SLOT(500),      55,     12,	0,
	"chain lightning",		"!Chain Lightning!",	"",NULL,NULL
    },

    {
	"change sex",
	{ 67, 77, 103, 103, 103, 75, 103,
	  57, 67, 103, 103, 103, 65, 103,
	47, 47, 47,		57, 57, 57,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	55, 55, 55,
	103, 103, 103 },
	{ 1, 2, 0, 0, 0, 1, 0,
	  1, 2, 0, 0, 0, 1, 0,
	1, 1, 1,	2, 2, 2,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0 },
	spell_change_sex,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(82),	15,	12,	0,
	"",			"Your body feels familiar again.",	"",NULL,NULL
    },

    {
	"charm person",
	{ 40, 30, 103, 103, 102, 60, 43,
	  30, 20, 103, 103, 102, 40, 33,
	20, 20, 20,		10, 10, 10,
	103, 103, 103,	103, 103, 103,
	102, 102, 102,	25, 25, 25,
	23, 23, 23 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_charm_person,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_charm_person,	TRUE,	SLOT( 7),	 5,	12,	0,
	"",			"You feel more self-confident.",	"",NULL,NULL
    },

    {
        "chi",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_chi,     TRUE,   SLOT( 0),       0,      0,	0,
        "chi",     "Your chi fades.",              "",NULL,NULL
    },

    {
	"chill touch",
	{ 12, 13, 103, 103, 103, 22, 40,
	   2,  3, 103, 103, 103, 12, 30,
	2, 2, 2,		3, 3, 3,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	7, 7, 7,
	20, 20, 20 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_chill_touch,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT( 8),	15,	12,	0,
	"chilling touch",	"You feel less cold.",	"",NULL,NULL
    },

    {
	"colour spray",
	{ 22, 33, 103, 103, 95, 20, 103,
	  12, 23, 103, 103, 85, 10, 103,
	8, 8, 8,		13, 13, 13,
	103, 103, 103,	103, 103, 103,
	75, 75, 75,		6, 6, 6,
	103, 103, 103 },
	{ 1, 1, 0, 0, 0, 1, 0,
	  1, 1, 0, 0, 0, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0 },
	spell_colour_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(10),	15,	12,	0,
	"colour spray",		"!Colour Spray!",	"",NULL,NULL
    },

    {
        "conjure",
        { 85, 90, 102, 102, 102, 95, 102,
          59, 80, 102, 102, 102, 71, 102,
	36, 36, 36,		70, 70, 70,
	102, 102, 102,	102, 102, 102,
	102, 102, 102,	56, 56, 56,
	102, 102, 102 },
        { 1, 1, 0, 0, 0, 1, 0,
          1, 1, 0, 0, 0, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0 },
        spell_conjure,        TAR_IGNORE,             POS_STANDING,
        NULL,          TRUE,         SLOT(280),       100,    12,	0,
        "",                     "!Conjure!",  "",NULL,NULL
    },

    {
	"conjure blizzard",
	{ 66, 103, 103, 103, 103, 86, 103,
	  55, 103, 103, 103, 103, 76, 103,
	44, 44, 44,		103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	66, 66, 66,
	103, 103, 103 },
	{ 1, 0, 0, 0, 0, 1, 1,
	  1, 0, 0, 0, 0, 1, 1,
	1, 1, 1,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_conjure_blizzard,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
        NULL,           TRUE,   SLOT(500),      175,     14,	0,
	"blizzard",		"!Conjure Blizzard!",	"",NULL,NULL
    },

    {
        "conjure wrath",
	{ 38, 44, 103, 103, 55, 44, 10,
	  28, 34, 103, 103, 45, 34, 1,
	18, 18, 18,		24, 24, 24,
	103, 103, 103,	103, 103, 103,
	35, 35, 35,		24, 24, 24,
	1, 1, 1 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
        spell_conjure_wrath,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,		TRUE,	SLOT(79),	100,	14,	0,
        "conjured wrath",               "!Conjure Wrath!",     "",NULL,NULL
    },


    {
	"continual light",
	{ 11, 8, 102, 102, 7, 5, 102,
	  1, 7, 102, 102, 1, 4, 102,
	1, 1, 1,		5, 5, 5,
	102, 102, 102, 	102, 102, 102,
	1, 1, 1,		2, 2, 2,
	102, 102, 102 },
	{ 1, 1, 0, 0, 1, 1, 0,
	  1, 1, 0, 0, 1, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0 },
	spell_continual_light,	TAR_IGNORE,		POS_STANDING,
	NULL,		TRUE,	SLOT(57),	 7,	12,	0,
	"",			"!Continual Light!",	"",NULL,NULL
    },

    {
	"control weather",
	{ 102, 48, 102, 102, 32, 20, 102,
	  102, 38, 102, 102, 22, 10, 102,
	102, 102, 102,	28, 28, 28,
	102, 102, 102,	102, 102, 102,
	12, 12, 12,		6, 6, 6,
	102, 102, 102 },
	{ 0, 1, 0, 0, 1, 1, 0,
	  0, 1, 0, 0, 1, 1, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1, 	1, 1, 1,
	0, 0, 0 },
	spell_control_weather,	TAR_IGNORE,		POS_STANDING,
	NULL,		TRUE,	SLOT(11),	25,	12,	0,
	"",			"!Control Weather!",	"",NULL,NULL
    },

    {
	"courage",
	{ 13, 2, 102, 40, 15, 23, 102,
	  3, 1, 102, 30,  5, 13, 102,
	2, 2, 2,		1, 1, 1,
	102, 102, 102,	20, 20, 20,
	3, 3, 3,		3, 3, 3,
	102, 102, 102 },
	{ 1, 1, 0, 2, 2, 1, 0,
	  1, 1, 0, 2, 2, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	2, 2, 2,
	2, 2, 2,	1, 1, 1,
	0, 0, 0 },
	spell_courage,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,		FALSE,	SLOT( 1),	 25,	12,	0,
	"",			"You feel less courageous.",	"",NULL,NULL
    },

    {
	"create food",
	{ 19, 10, 102, 102, 6, 7, 102,
	   9,  1, 102, 102, 1, 6, 102,
	6, 6, 6,		1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		4, 4, 4,
	102, 102, 102 },
	{ 1, 1, 0, 0, 1, 1, 0,
	  1, 1, 0, 0, 1, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0 },
	spell_create_food,	TAR_IGNORE,		POS_STANDING,
	NULL,		TRUE,	SLOT(12),	 5,	12,	0,
	"",			"!Create Food!",	"",NULL,NULL
    },

    {
	"create rose",
	{ 32, 21, 102, 102, 16, 11, 102,
	  22, 11, 102, 102,  6,  1, 102,
	12, 12, 12,		8, 8, 8,
	102, 102, 102,	102, 102, 102,
	3, 3, 3,		1, 1, 1,
	102, 102, 102 },
	{ 1, 1, 0, 0, 1, 1, 0,
	  1, 1, 0, 0, 1, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0, },
	spell_create_rose,	TAR_IGNORE,		POS_STANDING,
	NULL,		TRUE,	SLOT(511),	30, 	12,	0,
	"",			"!Create Rose!",	"",NULL,NULL
    },

    {
	"create spring",
	{ 28, 14, 102, 102, 26, 24, 102,
	  18,  4, 102, 102, 16, 14, 102,
	12, 12, 12,		2, 2, 2,
	102, 102, 102,	102, 102, 102,
	10, 10, 10,		8, 8, 8,
	102, 102, 102 },
	{ 1, 1, 0, 0, 1, 1, 0,
	  1, 1, 0, 0, 1, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0 },
	spell_create_spring,	TAR_IGNORE,		POS_STANDING,
	NULL,		TRUE,	SLOT(80),	20,	12,	0,
	"",			"!Create Spring!",	"",NULL,NULL
    },

    {
	"create water",
	{ 15, 5, 102, 102, 2, 3, 102,
	  5, 1, 102, 102, 1, 2, 102,
	3, 3, 3,		1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		2, 2, 2,
	102, 102, 102 },
	{ 1, 1, 0, 0, 1, 1, 0,
	  1, 1, 0, 0, 1, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0 },
	spell_create_water,	TAR_OBJ_INV,		POS_STANDING,
	NULL,		TRUE,	SLOT(13),	 5,	12,	0,
	"",			"!Create Water!",	"",NULL,NULL
    },

    {
	"cure blindness",
	{ 102, 11, 102, 102, 13, 16, 102,
	  90,   1, 102, 102,  3,  6, 102,
	80, 80, 80,		1, 1, 1,
	102, 102, 102,	102, 102, 102,
	2, 2, 2,		4, 4, 4,
	102, 102, 102 },
	{ 1, 1, 0, 0, 2, 1, 0,
	  1, 1, 0, 0, 2, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	0, 0, 0 },
	spell_cure_blindness,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,		FALSE,	SLOT(14),	 5,	12,	0,
	"",			"!Cure Blindness!",	"",NULL,NULL
    },

    {
	"cure critical",
	{ 102, 25, 102, 102, 49, 37, 102,
	  102, 15, 102, 102, 39, 27, 102,
	102, 102, 102,	10, 10, 10,
	102, 102, 102,	102, 102, 102,
	29, 29, 29,		17, 17, 17,
	102, 102, 102 },
	{ 0, 1, 0, 0, 2, 1, 0,
	  0, 1, 0, 0, 2, 1, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	0, 0, 0 },
	spell_cure_critical,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,		FALSE,	SLOT(15),	20,	12,	0,
	"",			"!Cure Critical!",	"",NULL,NULL
    },

    {
	"cure disease",
	{ 102, 26, 102, 102, 28, 32, 102,
	  102, 16, 102, 102, 18, 22, 102,
	102, 102, 102,	10, 10, 10,
	102, 102, 102,	102, 102, 102,
	12, 12, 12,		13, 13, 13,
	102, 102, 102 },
	{ 0, 1, 0, 0, 2, 1, 0,
	  0, 1, 0, 0, 2, 1, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	0, 0, 0 },
	spell_cure_disease,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		FALSE,	SLOT(501),	20,	12,	0,
	"",			"!Cure Disease!",	"",NULL,NULL
    },

    {
	"cure light",
	{ 102, 4, 102, 15, 3, 8, 102,
	  102, 3, 102, 5, 1, 1, 102,
	102, 102, 102,	2, 2, 2,
	102, 102, 102,	3, 3, 3,
	1, 1, 1,		1, 1, 1,
	102, 102, 102 },
	{ 0, 1, 0, 2, 2, 1, 0,
	  0, 1, 0, 2, 2, 1, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0,	2, 2, 2,
	2, 2, 2,	1, 1, 1,
	0, 0, 0 },
	spell_cure_light,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,		FALSE,	SLOT(16),	10,	12,	0,
	"",			"!Cure Light!",		"",NULL,NULL
    },

    {
	"cure poison",
	{ 102, 28, 102, 102, 37, 33, 102,
	  102, 18, 102, 102, 27, 23, 102,
	102, 102, 102,	10, 10, 10,
	102, 102, 102,	102, 102, 102,
	17, 17, 17,		13, 13, 13,
	102, 102, 102 },
	{ 0, 1, 0, 0, 2, 1, 0,
	  0, 1, 0, 0, 2, 1, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	0, 0, 0 },
	spell_cure_poison,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		FALSE,	SLOT(43),	 5,	12,	0,
	"",			"!Cure Poison!",	"",NULL,NULL
    },

    {
	"cure serious",
	{ 102, 14, 102, 60, 34, 24, 102,
	  102, 4, 102,  50, 24, 14, 102,
	102, 102, 102,	2, 2, 2,
	102, 102, 102,	40, 40, 40,
	14, 14, 14,		8, 8, 8,
	102, 102, 102 },
	{ 0, 1, 0, 2, 2, 1, 0,
	  0, 1, 0, 2, 2, 1, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0,	2, 2, 2,
	2, 2, 2,	1, 1, 1,
	0, 0, 0 },
	spell_cure_serious,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,		FALSE,	SLOT(61),	15,	12,	0,
	"",			"!Cure Serious!",	"",NULL,NULL
    },

    {
	"curse",
	{ 35, 35, 103, 103, 55, 38, 45,
	  25, 25, 103, 103, 45, 28, 35,
	15, 15, 15,		15, 15, 15,
	103, 103, 103,	103, 103, 103,
	35, 35, 35,		18, 18, 18,
	25, 25, 25 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_curse,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	&gsn_curse,	TRUE,	SLOT(17),	20,	12,	0,
	"curse",		"The curse wears off.",
	"$p is no longer impure.",NULL,NULL
    },

    {
	"deathshield",
	{ 102, 102, 102, 102, 102, 102, 90,
	  102, 102, 102, 102, 102, 102, 50,
	102, 102, 102,	102, 102, 102,
	102, 102, 102,	102, 102, 102,
	102, 102, 102,	102, 102, 102,
	20, 20, 20 },
	{ 0, 0, 0, 0, 0, 0, 1,
	  0, 0, 0, 0, 0, 0, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	1, 1, 1 },
	spell_deathshield,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,		FALSE,	SLOT(525),	75,	12,	0,
	"life drain",	"Your death shield fades away.",
	"",NULL,NULL
    },

    {
	"demonfire",
	{ 76, 68, 103, 103, 103, 103, 77,
	  66, 58, 103, 103, 103, 103, 67,
	56, 56, 56,		48, 48, 48,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	57, 57, 57 },
	{ 1, 1, 0, 0, 0, 0, 1,
	  1, 1, 0, 0, 0, 0, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	1, 1, 1 },
	spell_demonfire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(505),	50,	12,	0,
	"torments",		"!Demonfire!",		"",NULL,NULL
    },
    {
	"soul burn",
	{ 76, 68, 103, 103, 103, 103, 77,
	  66, 58, 103, 103, 103, 103, 67,
	56, 56, 56,		48, 48, 48,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	57, 57, 57 },
	{ 1, 1, 0, 0, 0, 0, 1,
	  1, 1, 0, 0, 0, 0, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	1, 1, 1 },
	spell_soul_burn,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(505),	50,	12,	0,
	"flames",		"!Soul Burn!",		"",NULL,NULL
    },

    {
	"detect evil",
	{ 22, 22, 102, 102, 10, 102, 17,
	  12, 12, 102, 102, 1, 85, 7,
	6, 6, 6,		6, 6, 6,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		75, 75, 75,
	4, 4, 4 },
	{ 1, 1, 0, 0, 1, 0, 1,
	  1, 1, 0, 0, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	spell_detect_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		FALSE,	SLOT(18),	 5,	12,	0,
	"",			"The red in your vision disappears.",	"",NULL,NULL
    },

    {
        "detect good",
	{ 23, 22, 102, 102, 10, 102, 17,
	  13, 12, 102, 102, 1, 102, 7,
	8, 8, 8,		7, 7, 7,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		102, 102, 102,
	3, 3, 3 },
	{ 1, 1, 0, 0, 1, 0, 1,
	  1, 1, 0, 0, 1, 0, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	0, 0, 0,
	1, 1, 1 },
        spell_detect_good,      TAR_CHAR_SELF,          POS_STANDING,
        NULL,		FALSE,	SLOT(513),        5,     12,	0,
        "",                     "The gold in your vision disappears.",	"",NULL,NULL
    },

    {
	"detect motion",
	{ 24, 22, 102, 102, 29, 26, 18,
	  14, 12, 102, 102, 19, 16, 8,
	7, 7, 7,		6, 6, 6,
	102, 102, 102,	102, 102, 102,
	12, 12, 12,		10, 10, 10,
	6, 6, 6 },
	{ 1, 1, 0, 0, 2, 1, 1,
	  1, 1, 0, 0, 2, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1 },
	spell_detect_motion,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		FALSE,	SLOT(44),	 10,	12,	0,
	"",			"You feel less aware of your surroundings.",
	"",NULL,NULL
    },

    {
	"detect invis",
	{ 25, 16, 102, 102, 25, 25, 4,
	  15, 6,  102, 102, 15, 15, 3,
	10, 10, 10,		4, 4, 4,
	102, 102, 102,	102, 102, 102,
	10, 10, 10,		10, 10, 10,
	2, 2, 2 },
	{ 1, 1, 0, 0, 1, 1, 1,
	  1, 1, 0, 0, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	spell_detect_invis,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		FALSE,	SLOT(19),	 10,	12,	0,
	"",			"You no longer see invisible objects.",
	"",NULL,NULL
    },

	{
	"detect location",
	{ 70, 73, 102, 102, 102, 102, 85,
	  60, 63, 102, 102, 102, 102, 75,
	50, 50, 50,		53, 53, 53,
	102, 102, 102,	102, 102, 102,
	102, 102, 102,	102, 102, 102,
	65, 65, 65 },
	{ 1, 1, 0, 0, 0, 0, 1,
	  1, 1, 0, 0, 0, 0, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0, 	0, 0, 0,
	1, 1, 1 },
	spell_detect_location,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		FALSE,	SLOT(600),	 5,	12,	0,
	"",			"Your sixth sense fades.",	"",NULL,NULL
    },

    {
	"detect magic",
	{ 26, 11, 102, 102, 24, 21, 3,
	  16,  1, 102, 102, 14, 11, 2,
	8, 8, 8,		1, 1, 1,
	102, 102, 102,	102, 102, 102,
	10, 10, 10,		8, 8, 8,
	1, 1, 1 },
	{ 1, 1, 0, 0, 1, 1, 1,
	  1, 1, 0, 0, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	spell_detect_magic,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		FALSE,	SLOT(20),	 5,	12,	0,
	"",			"The detect magic wears off.",	"",NULL,NULL
    },

    {
	"detect poison",
	{ 27, 14, 102, 102, 8, 11, 11,
	  17,  4, 102, 102, 1, 1, 1,
	11, 11, 11,		2, 2, 2,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 1, 1, 0, 0, 1, 1, 1,
	  1, 1, 0, 0, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	spell_detect_poison,	TAR_OBJ_INV,		POS_STANDING,
	NULL,		TRUE,	SLOT(21),	 5,	12,	0,
	"",			"!Detect Poison!",	"",NULL,NULL
    },

    {
	"dispel evil",
	{ 103, 25, 103, 103, 103, 35, 53,
	  103, 15, 103, 103, 103, 25, 43,
	103, 103, 103,	10, 10, 10,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	15, 15, 15,
	33, 33, 33 },
	{ 0, 1, 0, 0, 0, 1, 2,
	  0, 1, 0, 0, 0, 1, 2,
	0, 0, 0,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	2, 2, 2 },
	spell_dispel_evil,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(22),	15,	12,	0,
	"dispel evil",		"!Dispel Evil!",	"",NULL,NULL
    },

    {
        "dispel good",
	{ 103, 30, 43, 103, 103, 35, 53,
	  103, 20, 33, 103, 103, 25, 43,
	103, 103, 103,	12, 12, 12,
	23, 23, 23,		103, 103, 103,
	103, 103, 103,	15, 15, 15,
	33, 33, 33 },
	{ 0, 1, 2, 0, 0, 1, 2,
	  0, 1, 2, 0, 0, 1, 2,
	0, 0, 0,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	2, 2, 2 },
        spell_dispel_good,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,		TRUE,	SLOT(512),      15,     12,	0,
        "dispel good",          "!Dispel Good!",	"",NULL,NULL
    },

	{
	"dispel invis",
	{ 22, 22, 102, 102, 22, 22, 22,
	  12, 12, 102, 102, 12, 12, 12,
	7, 7, 7,		7, 7, 7,
	102, 102, 102,	102, 102, 102,
	7, 7, 7,		7, 7, 7,
	7, 7, 7 },
	{ 1, 1, 0, 0, 2, 1, 2,
	  1, 1, 0, 0, 2, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2 },
	spell_dispel_invis,	TAR_OBJ_INV,	POS_STANDING,
	NULL,		TRUE,	SLOT(559),	15,	12,	0,
	"",			"!Dispel Invis!",	"",NULL,NULL
    },

    {
	"dispel magic",
	{ 48, 48, 102, 90, 58, 36, 60,
	  38, 38, 102, 80, 48, 35, 50,
	28, 28, 28,		28, 28, 28,
	102, 102, 102,	70, 70, 70,
	38, 38, 38,		30, 30, 30,
	40, 40, 40 },
	{ 1, 1, 0, 2, 0, 1, 2,
	  1, 1, 0, 2, 0, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	2, 2, 2,
	0, 0, 0,	1, 1, 1,
	2, 2, 2 },
	spell_dispel_magic,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(59),	15,	12,	0,
	"",			"!Dispel Magic!",	"",NULL,NULL
    },

	{
	"displace",
	{ 70, 73, 102, 102, 102, 80, 85,
	  60, 63, 102, 102, 102, 70, 75,
	50, 50, 50,		53, 53, 53,
	102, 102, 102,	102, 102, 102,
	102, 102, 102,	60, 60, 60,
	65, 65, 65 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_displace,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		FALSE,	SLOT(601),	 5,	12,	0,
	"",			"You are where you thought you are.",	"",NULL,NULL
    },

    {
	"earthquake",
	{ 83, 20, 103, 103, 36, 11, 76,
	  73, 10, 103, 103, 26, 1, 66,
	63, 63, 63,		5, 5, 5,
	103, 103, 103,	103, 103, 103,
	16, 16, 16,		1, 1, 1,
	56, 56, 56 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_earthquake,	TAR_IGNORE,		POS_FIGHTING,
	NULL,		TRUE,	SLOT(23),	15,	12,	0,
	"earthquake",		"!Earthquake!",		"",NULL,NULL
    },

    {
	"empower",
	{ 36, 40, 103, 103, 102, 42, 103,
	  26, 30, 103, 103, 102, 32, 103,
	16, 16, 16,		20, 20, 20,
	103, 103, 103,	103, 103, 103,
	102, 102, 102,	22, 22, 22,
	103, 103, 103 },
	{ 1, 1, 0, 0, 0, 1, 0,
	  1, 1, 0, 0, 0, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0 },
	spell_empower,		TAR_IGNORE,	POS_RESTING,
	NULL,		TRUE,	SLOT(234),	 40,	16,	0,
	"",			"!Empower!",	"",NULL,NULL
    },

    {
	"enchant armor",
	{ 31, 90, 102, 102, 102, 102, 102,
	  21, 80, 102, 102, 101, 101, 102,
	12, 12, 12,		70, 70, 70,
	102, 102, 102,	102, 102, 102,
	101, 101, 101,	101, 101, 101,
	102, 102, 102 },
	{ 2, 2, 0, 0, 0, 0, 4,
	  2, 2, 0, 0, 3, 2, 4,
	2, 2, 2,	2, 2, 2,
	0, 0, 0,	0, 0, 0,
	3, 3, 3,	2, 2, 2,
	4, 4, 4 },
	spell_enchant_armor,	TAR_OBJ_INV,		POS_STANDING,
	NULL,		TRUE,	SLOT(510),	100,	24,	0,
	"",			"!Enchant Armor!",	"",NULL,NULL
    },

    {
	"enchant weapon",
	{ 32, 70, 102, 102, 102, 102, 102,
	  22, 60, 102, 102, 101, 101, 102,
	12, 12, 12,		50, 50, 50,
	102, 102, 102,	102, 102, 102,
	101, 101, 101,	101, 101, 101,
	102, 102, 102 },
	{ 2, 2, 0, 0, 0, 0, 0,
	  2, 2, 0, 0, 3, 2, 0,
	2, 2, 2,	2, 2, 2,
	0, 0, 0,	0, 0, 0,
	3, 3, 3,	2, 2, 2,
	0, 0, 0 },
	spell_enchant_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,		TRUE,	SLOT(24),	100,	24,	0,
	"",			"!Enchant Weapon!",	"",NULL,NULL
    },

    {
	"energy drain",
	{ 38, 44, 103, 103, 55, 44, 10,
	  28, 34, 103, 103, 45, 34, 1,
	18, 18, 18,		24, 24, 24,
	103, 103, 103,	103, 103, 103,
	35, 35, 35,		24, 24, 24,
	1, 1, 1 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_energy_drain,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(25),	35,	12,	0,
	"energy drain",		"!Energy Drain!",	"",NULL,NULL
    },

    {
	"entity",
	{ 38, 44, 103, 103, 55, 44, 10,
	  28, 34, 103, 103, 45, 34, 1,
	18, 18, 18,		24, 24, 24,
	103, 103, 103,	103, 103, 103,
	35, 35, 35,		24, 24, 24,
	1, 1, 1 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_entity,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
        NULL,           TRUE,   SLOT(79),       200,     24,	0,
	"entity",		"!Entity!",	"",NULL,NULL
    },

    {
	"faerie fog",
	{ 28, 41, 103, 103, 35, 18, 103,
	  18, 31, 103, 103, 25, 8, 103,
	12, 12, 12,		21, 21, 21,
	103, 103, 103,	103, 103, 103,
	15, 15, 15,		4, 4, 4,
	103, 103, 103 },
	{ 1, 1, 0, 0, 1, 1, 2,
	  1, 1, 0, 0, 1, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	2, 2, 2 },
	spell_faerie_fog,	TAR_IGNORE,		POS_STANDING,
	NULL,		TRUE,	SLOT(73),	12,	12,	0,
        "faerie fog",           "!Faerie Fog!",         "The purple fog dissipates.\n\r",NULL,NULL
    },

    {
	"farsight",
	{ 4, 8, 103, 103, 15, 9, 6,
	  3, 7, 103, 103, 5, 8, 5,
	2, 2, 2,		5, 5, 5,
	103, 103, 103,	103, 103, 103,
	3, 3, 3,		6, 6, 6,
	3, 3, 3 },
	{ 1, 1, 0, 0, 1, 1, 1,
	  1, 1, 0, 0, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	spell_farsight,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		FALSE,	SLOT(521),	5,	12,	0,
	"",			"The green in your vision disappears.",
	"",NULL,NULL
    },

    {
	"feeble mind",
	{ 103, 35, 103, 103, 103, 103, 103,
	  103, 25, 103, 103, 103, 103, 103,
	103, 103, 103,	15, 15, 15,
	103, 103, 103,	103, 103, 103,
	103, 103, 103, 	103, 103, 103,
	103, 103, 103 },
	{ 0, 1, 0, 0, 0, 0, 0,
	  0, 1, 0, 0, 0, 0, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_feeble_mind,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,	TRUE,	SLOT(417),	20,	12,	0,
	"",		"You don't feel so stupid.",
	"",NULL,NULL
    },


    {
	"fireball",
	{ 44, 89, 103, 103, 79, 44, 81,
	  34, 79, 103, 103, 69, 34, 71,
	24, 24, 24,		69, 69, 69,
	103, 103, 103,	103, 103, 103,
	59, 59, 59,		24, 24, 24,
	61, 61, 61 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_fireball,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(26),	15,	12,	0,
	"fireball",		"!Fireball!",		"",NULL,NULL
    },

    {
	"fireproof",
	{ 25, 24, 102, 102, 35, 21, 42,
	  15, 14, 102, 102, 25, 11, 32,
	10, 10, 10,		9, 9, 9,
	102, 102, 102,	102, 102, 102,
	15, 15, 15,		7, 7, 7,
	22, 22, 22 },
	{ 1, 1, 0, 0, 2, 1, 2,
	  1, 1, 0, 0, 2, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2 },
	spell_fireproof,	TAR_OBJ_INV,		POS_STANDING,
	NULL,		TRUE,	SLOT(523),	10,	12,	0,
	"",			"",	"$p's protective aura fades.",NULL,NULL
    },

    {
        "fires",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_fires,     TRUE,   SLOT(999),       0,      0,	0,
        "",     "The fires subside.",              "",NULL,NULL
    },

    {
	"fireshield",
	{ 50, 60, 102, 102, 102, 55, 102,
	  40, 50, 102, 102, 102, 45, 102,
	30, 30, 30,		40, 40, 40,
	102, 102, 102,	102, 102, 102,
	102, 102, 102,	35, 35, 35,
	102, 102, 102 },
	{ 3, 3, 0, 0, 0, 3, 5,
	  3, 3, 0, 0, 0, 3, 5,
	3, 3, 3,	3, 3, 3,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	3, 3, 3,
	5, 5, 5 },
	spell_fireshield,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		FALSE,	SLOT(411),	75,	12,	0,
	"fireball",		"Your firey shield gutters out.",
	"",NULL,NULL
    },

    {
	"flamestrike",
	{ 34, 40, 103, 103, 45, 30, 57,
	  24, 30, 103, 103, 35, 20, 47,
	14, 14, 14,		20, 20, 20,
	103, 103, 103,	103, 103, 103,
	25, 25, 25,		10, 10, 10,
	37, 37, 37 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_flamestrike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(65),	20,	12,	0,
	"flamestrike",		"!Flamestrike!",		"",NULL,NULL
    },

    {
	"floating disc",
	{ 7, 19, 102, 102, 45, 15, 102,
	  6,  9, 102, 102, 35,  5, 102,
	4, 4, 4,		7, 7, 7,
	102, 102, 102,	102, 102, 102,
	25, 25, 25,		3, 3, 3,
	102, 102, 102 },
	{ 1, 1, 0, 0, 1, 1, 0,
	  1, 1, 0, 0, 1, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0 },
	spell_floating_disc,	TAR_IGNORE,		POS_STANDING,
	NULL,		TRUE,	SLOT(522),	40,	24,	0,
	"",			"!Floating disc!",	"",NULL,NULL
    },

    {
	"fly",
	{ 19, 36, 102, 102, 31, 66, 33,
	   9, 26, 102, 102, 21, 56, 23,
	7, 7, 7,		16, 16, 16,
	102, 102, 102,	102, 102, 102,
	11, 11, 11,		46, 46, 46,
	13, 13, 13 },
	{ 1, 1, 0, 0, 2, 1, 1,
	  1, 1, 0, 0, 2, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1 },
	spell_fly,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		FALSE,	SLOT(56),	10,	18,	0,
	"",			"You slowly float to the ground.",	"",NULL,NULL
    },
    
    {
	"fortify strength",
	{ 102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102 },
	{   1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1 },
	    
	spell_fortify_strength,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,		     FALSE,	    SLOT(500),	   200,	 12,   0,
	"",			"Your strength becomes unfortified.",	"",NULL,NULL
    },

    {
	"fortify intelligence",
	{ 102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102 },
	{   1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1 },
	    
	spell_fortify_intelligence,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,		     FALSE,	    SLOT(500),	   200,	 12,   0,
	"",			"Your intelligence becomes unfortified.",	"",NULL,NULL
    },

    {
	"fortify wisdom",
	{ 102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102 },
	{   1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1 },
	    
	spell_fortify_wisdom,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,		     FALSE,	    SLOT(500),	   200,	 12,   0,
	"",			"Your wisdom becomes unfortified.",	"",NULL,NULL
    },
    
    {
	"fortify dexterity",
	{ 102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102 },
	{   1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1 },
	    
	spell_fortify_dexterity,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,		     FALSE,	    SLOT(500),	   200,	 12,   0,
	"",			"Your dexterity becomes unfortified.",	"",NULL,NULL
    },    
    
    {
	"fortify constitution",
	{ 102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102 },
	{   1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1 },
	    
	spell_fortify_constitution,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,		     FALSE,	    SLOT(500),	   200,	 12,   0,
	"",			"Your constitution becomes unfortified.",	"",NULL,NULL
    },
    
    {
        "frenzy",
	{ 46, 48, 103, 103, 57, 57, 55,
	  36, 38, 103, 103, 47, 47, 45,
	26, 26, 26,		28, 28, 28,
	103, 103, 103,	103, 103, 103,
	37, 37, 37,		37, 37, 37,
	35, 35, 35 },
	{ 1, 1, 0, 0, 2, 1, 2,
	  1, 1, 0, 0, 2, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2 },
        spell_frenzy,           TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,		FALSE,	SLOT(504),      30,     24,	0,
        "",                     "Your rage ebbs.",	"",NULL,NULL
    },

    {
	"gate",
	{ 54, 34, 102, 102, 42, 61, 56,
	  44, 24, 102, 102, 32, 51, 46,
	34, 34, 34,		14, 14, 14,
	102, 102, 102,	102, 102, 102,
	22, 22, 22,		41, 41, 41,
	36, 36, 36 },
	{ 1, 1, 0, 0, 2, 1, 1,
	  1, 1, 0, 0, 2, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1 },
	spell_gate,		TAR_IGNORE,		POS_FIGHTING,
	NULL,		FALSE,	SLOT(83),	80,	12,	0,
	"",			"!Gate!",		"",NULL,NULL
    },

    {
	"giant strength",
	{ 41, 42, 102, 102, 37, 31, 37,
	  31, 32, 102, 102, 27, 21, 27,
	21, 21, 21,		22, 22, 22,
	102, 102, 102,	102, 102, 102,
	17, 17, 17,		11, 11, 11,
	17, 17, 17 },
	{ 1, 1, 0, 0, 2, 1, 2,
	  1, 1, 0, 0, 2, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2 },
	spell_giant_strength,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,		FALSE,	SLOT(39),	20,	12,	0,
	"",			"You feel weaker.",	"",NULL,NULL
    },

    {
	"harm",
	{ 43, 46, 103, 103, 63, 41, 83,
	  33, 36, 103, 103, 53, 31, 73,
	23, 23, 23,		26, 26, 26,
	103, 103, 103,	103, 103, 103,
	43, 43, 43,		21, 21, 21,
	63, 63, 63 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_harm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(27),	35,	12,	0,
	"harm spell",		"!Harm!,		""",NULL,NULL
    },

    {
	"haste",
	{ 42, 54, 103, 103, 60, 26, 52,
	  32, 44, 103, 103, 50, 16, 42,
	22, 22, 22,		34, 34, 34,
	103, 103, 103,	103, 103, 103,
	40, 40, 40,		10, 10, 10,
	32, 32, 32 },
	{ 1, 1, 0, 2, 2, 1, 1,
	  1, 1, 0, 2, 2, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	2, 2, 2,
	2, 2, 2,	1, 1, 1,
	1, 1, 1 },
	spell_haste,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,		FALSE,	SLOT(502),	30,	12,	0,
	"",			"You feel yourself slow down.",	"",NULL,NULL
    },

    {
	"heal",
	{ 102, 32, 102, 102, 102, 55, 102,
	  102, 22, 102, 102, 102, 45, 102,
	102, 102, 102,	12, 12, 12,
	102, 102, 102,	102, 102, 102,
	102, 102, 102,	35, 35, 35,
	102, 102, 102 },
	{ 0, 1, 0, 0, 0, 1, 0,
	  0, 1, 0, 0, 0, 1, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0 },
	spell_heal,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,		FALSE,	SLOT(28),	50,	12,	0,
	"",			"!Heal!",		"",NULL,NULL
    },

    {
	"heat metal",
	{ 43, 42, 103, 103, 102, 22, 103,
	  33, 32, 103, 103, 102, 12, 103,
	23, 23, 23,		22, 22, 22,
	103, 103, 103,	103, 103, 103,
	102, 102, 102,	7, 7, 7,
	103, 103, 103, },
	{ 1, 1, 0, 0, 0, 1, 0,
	  1, 1, 0, 0, 0, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0 },
	spell_heat_metal,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(516), 	25,	16,	0,
	"spell",		"!Heat Metal!",		"",NULL,NULL
    },

    {
	"holy word",
	{ 103, 71, 103, 103, 103, 77, 103,
	  103, 61, 103, 103, 103, 67, 103,
	103, 103, 103,	51, 51, 51,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	57, 57, 57,
	103, 103, 103 },
	{ 0, 2, 0, 0, 0, 2, 0,
	  0, 2, 0, 0, 0, 2, 0,
	0, 0, 0,	2, 2, 2,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	2, 2, 2,
	0, 0, 0 },
	spell_holy_word,	TAR_IGNORE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(506), 	200,	24,	0,
	"divine wrath",		"!Holy Word!",		"",NULL,NULL
    },

    {
	"iceshield",
	{ 30, 30, 102, 102, 102, 35, 102,
	  20, 20, 102, 102, 102, 25, 102,
	10, 10, 10,		10, 10, 10,
	102, 102, 102,	102, 102, 102,
	102, 102, 102,	15, 15, 15,
	102, 102, 102 },
	{ 3, 3, 0, 0, 0, 3, 0,
	  3, 3, 0, 0, 0, 3, 0,
	3, 3, 3,	3, 3, 3,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	3, 3, 3,
	0, 0, 0 },
	spell_iceshield,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		FALSE,	SLOT(410),	75,	12,	0,
	"chilling touch",	"Your icy shield slowly melts away.",
	"",NULL,NULL
    },

    {
	"identify",
	{ 30, 32, 103, 102, 46, 42, 28,
	  20, 22, 103, 102, 36, 32, 18,
	10, 10, 10,		12, 12, 12,
	103, 103, 103,	102, 102, 102,
	26, 26, 26,		22, 22, 22,
	12, 12, 12 },
	{ 1, 1, 0, 0, 1, 1, 1,
	  1, 1, 0, 0, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	1, 1, 1, },
	spell_identify,		TAR_OBJ_INV,		POS_STANDING,
	NULL,		TRUE,	SLOT(53),	12,	6,	0,
	"",			"!Identify!",		"",NULL,NULL
    },

    {
	"infravision",
	{ 18, 26, 102, 102, 20, 18, 12,
	   8, 16, 102, 102, 10,  8,  2,
	5, 5, 5,		10, 10, 10,
	102, 102, 102,	102, 102, 102,
	6, 6, 6,		5, 5, 5,
	1, 1, 1 },
	{ 1, 1, 0, 0, 2, 1, 2,
	  1, 1, 0, 0, 2, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2 },
	spell_infravision,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		FALSE,	SLOT(77),	 5,	18,	0,
	"",			"You no longer see in the dark.",	"",NULL,NULL
    },
    /*
    {
	"invigorate pith",
	{ 102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102 },
	{   1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1 },
	    
	spell_invigorate_pith,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,		     FALSE,	    SLOT(500),	   200,	 12,   0,
	"",			"You invigorate your pith.",	""
    },
    
    {
	"invigorate mind",
	{ 102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102 },
	{   1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1 },
	    
	spell_invigorate_mind,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,		     FALSE,	    SLOT(500),	   200,	 12,   0,
	"",			"You invigorate your mind.",	""
    },
    
    {
	"invigorate lore",
	{ 102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102 },
	{   1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1 },
	    
	spell_invigorate_lore,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,		     FALSE,	    SLOT(500),	   200,	 12,   0,
	"",			"You invigorate your lore.",	""
    },
    
    {
	"invigorate body",
	{ 102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102 },
	{   1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1 },
	    
	spell_invigorate_body,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,		     FALSE,	    SLOT(500),	   200,	 12,   0,
	"",			"You invigorate your body.",	""
    },
    
    {
	"invigorate tact",
	{ 102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102 },
	{   1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1,
	    1,   1,   1,   1,   1,   1,   1 },
	    
	spell_invigorate_tact,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,		     FALSE,	    SLOT(500),	   200,	 12,   0,
	"",			"You invigorate your tact.",	""
    },
    */
    {
	"invisibility",
	{ 9, 37, 103, 102, 12, 39, 13,
	  8, 27, 103, 102,  2, 29,  3,
	6, 6, 6,		17, 17, 17,
	103, 103, 103,	102, 102, 102,
	1, 1, 1,		19, 19, 19,
	2, 2, 2 },
	{ 1, 1, 0, 0, 2, 1, 1,
	  1, 1, 0, 0, 2, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1 },
	spell_invis,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_invis,	FALSE,	SLOT(29),	 5,	12,	0,
	"",			"You are no longer invisible.",
	"$p fades into view.",NULL,NULL
    },

    {
	"know alignment",
	{ 24, 17, 102, 102, 36, 27, 33,
	  14,  7, 102, 102, 26, 17, 23,
	10, 10, 10,		5, 5, 5,
	102, 102, 102,	102, 102, 102,
	16, 16, 16,		11, 11, 11,
	13, 13, 13 },
	{ 1, 1, 0, 0, 1, 1, 1,
	  1, 1, 0, 0, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	spell_know_alignment,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,		FALSE,	SLOT(58),	 9,	12,	0,
	"",			"!Know Alignment!",	"",NULL,NULL
    },

    {
	"lightning bolt",
	{ 25, 46, 103, 103, 40, 42, 91,
	  15, 36, 103, 103, 30, 32, 81,
	10, 10, 10,		26, 26, 26,
	103, 103, 103,	103, 103, 103,
	20, 20, 20,		22, 22, 22,
	71, 71, 71 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_lightning_bolt,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(30),	15,	12,	0,
	"lightning bolt",	"!Lightning Bolt!",	"",NULL,NULL
    },

    {
	"locate object",
	{ 17, 30, 102, 102, 38, 35, 24,
	   7, 20, 102, 102, 28, 25, 14,
	5, 5, 5,		12, 12, 12,
	102, 102, 102,	102, 102, 102,
	18, 18, 18,		15, 15, 15,
	10, 10, 10 },
	{ 1, 1, 0, 0, 1, 1, 1,
	  1, 1, 0, 0, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	spell_locate_object,	TAR_IGNORE,		POS_STANDING,
	NULL,		TRUE,	SLOT(31),	20,	18,	0,
     "",            "!Locate Object!",  "",NULL,NULL
    },

    {
	"magic missile",
	{ 1, 103, 103, 103, 103, 12, 103,
	  1, 103, 103, 103, 103,  2, 103,
	1, 1, 1,		103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	1, 1, 1,
	103, 103, 103 },
	{ 1, 0, 0, 0, 0, 1, 0,
	  1, 1, 0, 0, 0, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0 },
	spell_magic_missile,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(32),	15,	6,	0,
	"magic missile",	"!Magic Missile!",	"",NULL,NULL
    },

    {
	"mana blast",
	{ 55, 93, 103, 103, 102, 45, 65,
	  45, 83, 103, 103, 102, 35, 55,
	35, 35, 35,		73, 73, 73,
	103, 103, 103,	103, 103, 103,
	102, 102, 102,	25, 25, 25,
	45, 45, 45 },
	{ 1, 1, 0, 0, 2, 1, 1,
	  1, 1, 0, 0, 2, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1 },
	spell_mana_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(0),	50,	12,	0,
	"mana blast",	"!Mana Blast!",		"",NULL,NULL
    },

    {
	"martyr",
	{ 110, 110, 110, 110, 110, 110, 110,
	  110, 110, 110, 110, 110, 110, 110,
	110, 110, 110,	110, 110, 110,
	110, 110, 110,	110, 110, 110,
	110, 110, 110,	110, 110, 110,
	110, 110, 110 },
	{ 1, 1, 1, 1, 1, 1, 1,
	  1, 1, 1, 1, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	spell_martyr,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,		TRUE,	SLOT(0),	 50,	6,	0,
	"",	"You feel ready to martyr yourself again.",	"",NULL,NULL
    },
    
    {
	"mass healing",
	{ 92, 75, 102, 102, 85, 85, 102,
	  82, 65, 102, 102, 75, 75, 102,
	72, 72, 72,		55, 55, 55,
	102, 102, 102,	102, 102, 102,
	65, 65, 65,		65, 65, 65,
	102, 102, 102 },
	{ 2, 2, 0, 0, 2, 2, 0,
	  2, 2, 0, 0, 2, 2, 0,
	2, 2, 2,	2, 2, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	2, 2, 2,
	0, 0, 0 },
	spell_mass_healing,	TAR_IGNORE,		POS_STANDING,
	NULL,		FALSE,	SLOT(508),	100,	36,	0,
	"",			"!Mass Healing!",	"",NULL,NULL
    },

    {
	"mass invis",
	{ 43, 49, 103, 103, 55, 55, 103,
	  33, 39, 103, 103, 45, 45, 103,
	23, 23, 23,		29, 29, 29,
	103, 103, 103,	103, 103, 103,
	35, 35, 35,		35, 35, 35,
	103, 103, 103 },
	{ 1, 1, 2, 0, 0, 1, 0,
	  1, 1, 2, 0, 0, 1, 3,
	1, 1, 1,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	3, 3, 3 },
	spell_mass_invis,	TAR_IGNORE,		POS_STANDING,
	&gsn_mass_invis,	FALSE,	SLOT(69),	20,	24,	0,
	"",			"You are no longer invisible.",		"",NULL,NULL
    },

/*    {
    "quench",
   { 79, 69, 102, 102, 81, 85, 92,
          69, 59, 102, 102, 71, 75, 82,
        59, 59, 59,             49, 49, 49,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,                1, 1, 1,
        1, 1, 1 },
        { 2, 2, 0, 0, 0, 2, 4,
          2, 2, 0, 0, 0, 2, 4,
        2, 2, 2,        2, 2, 2,
        0, 0, 0,        0, 0, 0,
        0, 0, 0,        2, 2, 2,
        4, 4, 4 },
        spell_quench,            TAR_IGNORE,             POS_STANDING,
        NULL,           FALSE,  SLOT(520),       150,   36,	0,
        "",                     "!quench!",              "",NULL,NULL
    },


    {
        "sate",
        { 79, 69, 102, 102, 81, 85, 92,
          69, 59, 102, 102, 71, 75, 82,
        59, 59, 59,             49, 49, 49,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,                1, 1, 1,
        1, 1, 1 },
        { 2, 2, 0, 0, 0, 2, 4,
          2, 2, 0, 0, 0, 2, 4,
        2, 2, 2,        2, 2, 2,
        0, 0, 0,        0, 0, 0,
        0, 0, 0,        2, 2, 2,
        4, 4, 4 },
        spell_sate,            TAR_IGNORE,             POS_STANDING,
        NULL,           FALSE,  SLOT(520),       150,   36,	0,
        "",                     "!sate!",              ""
    },*/


    {
        "nexus",
	{ 79, 69, 102, 102, 81, 85, 92,
	  69, 59, 102, 102, 71, 75, 82,
	59, 59, 59,		49, 49, 49,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 2, 2, 0, 0, 0, 2, 4,
	  2, 2, 0, 0, 0, 2, 4,
	2, 2, 2,	2, 2, 2,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	2, 2, 2,
	4, 4, 4 },
        spell_nexus,            TAR_IGNORE,             POS_STANDING,
        NULL,		FALSE,	SLOT(520),       150,   36,	0,
        "",                     "!Nexus!",		"",NULL,NULL
    },

    {
	"pass door",
	{ 48, 63, 103, 102, 60, 73, 42,
	  38, 53, 103, 102, 50, 63, 32,
	28, 28, 28,		43, 43, 43,
	103, 103, 103,	102, 102, 102,
	40, 40, 40,		53, 53, 53,
	22, 22, 22 },
	{ 1, 1, 0, 0, 2, 1, 1,
	  1, 1, 0, 0, 2, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1 },
	spell_pass_door,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		FALSE,	SLOT(74),	20,	12,	0,
	"",			"You feel solid again.",	"",NULL,NULL
    },

    {
	"plague",
	{ 46, 34, 103, 103, 59, 39, 65,
	  36, 24, 103, 103, 49, 29, 55,
	26, 26, 26,		14, 14, 14,
	103, 103, 103,	103, 103, 103,
	39, 39, 39,		19, 19, 19,
	45, 45, 45 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_plague,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_plague,	TRUE,	SLOT(503),	20,	12,	0,
	"sickness",		"Your sores vanish.",	"",NULL,NULL
    },

    {
	"poison",
	{ 33, 24, 103, 103, 49, 27, 22,
	  23, 14, 103, 103, 39, 17, 12,
	13, 13, 13,		10, 10, 10,
	103, 103, 103,	103, 103, 103,
	29, 29, 29,		11, 11, 11,
	8, 8, 8 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_poison,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	&gsn_poison,	TRUE,	SLOT(33),	10,	12,	0,
	"poison"		"You feel less sick.",	"",NULL,NULL
    },
    {
	"voodan curse",
	{ 33, 24, 103, 103, 49, 27, 22,
	  23, 14, 103, 103, 39, 17, 12,
	13, 13, 13,		10, 10, 10,
	103, 103, 103,	103, 103, 103,
	29, 29, 29,		11, 11, 11,
	8, 8, 8 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_voodan_curse,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	NULL,	TRUE,	SLOT(33),	10,	12,	0,
	""		"You feel less sick.",	"",NULL,NULL
    },

    {
        "portal",
	{ 70, 59, 102, 102, 72, 102, 83,
	  60, 49, 102, 102, 62, 102, 73,
	50, 50, 50,		39, 39, 39,
	102, 102, 102,	102, 102, 102,
	52, 52, 52,		102, 102, 102,
	63, 63, 63 },
	{ 2, 2, 0, 0, 0, 0, 4,
	  2, 2, 0, 0, 0, 2, 4,
	2, 2, 2,	2, 2, 2,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	2, 2, 2,
	4, 4, 4 },
        spell_portal,           TAR_IGNORE,             POS_STANDING,
        NULL,		FALSE,	SLOT(519),       100,     24,	0,
        "",                     "!Portal!",		"",NULL,NULL
    },

	{
    "prismatic spray",
	{ 75, 103, 103, 103, 103, 103, 103,
	  65, 103, 103, 103, 103, 103, 103,
	55, 55, 55,		103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103 },
	{ 1, 0, 0, 0, 0, 0, 0,
	  1, 0, 0, 0, 0, 0, 0,
	1, 1, 1,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
    spell_prismatic_spray,     TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
    NULL,		TRUE,	SLOT(602),      20,     12,	0,
    "colorful hues",         "The color leaves your eyes.",	"",NULL,NULL
    },

    {
	"protection evil",
	{ 24, 18, 102, 103, 64, 19, 66,
	  14, 8,  102, 103, 54, 9, 56,
	10, 10, 10,		6, 6, 6,
	102, 102, 102,	103, 103, 103,
	44, 44, 44,		7, 7, 7,
	46, 46, 46 },
     { 1, 1, 0, 0, 1, 1, 2,
       1, 1, 0, 0, 1, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	2, 2, 2 },
	spell_protection_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		FALSE,	SLOT(34), 	5,	12,	0,
	"",			"You feel less protected.",	"",NULL,NULL
    },

    {
        "protection good",
	{ 24, 18, 102, 103, 64, 19, 66,
	  14,  8, 102, 103, 54,  9, 56,
	10, 10, 10,		6, 6, 6,
	102, 102, 102,	103, 103, 103,
	44, 44, 44,		7, 7, 7,
	46, 46, 46 },
	{ 1, 1, 0, 0, 2, 1, 2,
	  1, 1, 0, 0, 2, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2 },
        spell_protection_good,  TAR_CHAR_SELF,          POS_STANDING,
        NULL,		FALSE,	SLOT(514),	5,	12,	0,
        "",                     "You feel less protected.",	"",NULL,NULL
    },

    {
	"protection voodoo",
	{ 102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	102, 102, 102,	102, 102, 102,
	102, 102, 102, 	102, 102, 102,
	102, 102, 102,	102, 102, 102,
	102, 102, 102 },
	{ 1, 1, 1, 1, 1, 1, 1,
	  1, 1, 1, 1, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	spell_protection_voodoo, TAR_CHAR_SELF,		POS_STANDING,
	NULL,		FALSE,	SLOT(345),	5,	12,	0,
	"",			"You feel exposed.",	"",NULL,NULL
    },

    {
	"quest pill",
	{ 110, 110, 110, 110, 110, 110, 110,
	  110, 110, 110, 110, 110, 110, 110,
	110, 110, 110,	110, 110, 110,
	110, 110, 110,	110, 110, 110,
	110, 110, 110,	110, 110, 110,
	110, 110, 110 },
	{ 1, 1, 1, 1, 1, 1, 1,
	  1, 1, 1, 1, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	spell_quest_pill,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		TRUE,	SLOT(530),	 5,	18,	0,
	"",			"!Quest Pill!",	"",NULL,NULL
    },

    {
        "ray of truth",
	{ 103, 49, 103, 103, 98, 76, 103,
	  103, 39, 103, 103, 88, 66, 103,
	103, 103, 103,	39, 39, 39,
	103, 103, 103,	103, 103, 103,
	78, 78, 78,		56, 56, 56,
	103, 103, 103 },
	{ 0, 1, 0, 0, 0, 1, 0,
	  0, 1, 0, 0, 0, 1, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0, 	1, 1, 1,
	0, 0, 0 },
        spell_ray_of_truth,     TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           TRUE,   SLOT(518),      30,     16,	0,
        "ray of truth",         "!Ray of Truth!",	"",NULL,NULL
    },

    {
	"recharge",
	{ 18, 63, 103, 103, 103, 52, 103,
	   8, 53, 103, 103, 103, 42, 103,
	6, 6, 6,		43, 43, 43,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	32, 32, 32,
	103, 103, 103 },
	{ 1, 1, 0, 0, 0, 1, 0,
	  1, 1, 0, 0, 0, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0 },
	spell_recharge,		TAR_OBJ_INV,		POS_STANDING,
	NULL,		TRUE,	SLOT(517),	60,	24,	0,
	"",			"!Recharge!",		"",NULL,NULL
    },

    {
	"refresh",
	{ 16, 9, 102, 102, 18, 10, 16,
	   6, 4, 102, 102,  8,  1,  6,
	4, 4, 4,		2, 2, 2,
	102, 102, 102,	102, 102, 102,
	6, 6, 6,		1, 1, 1,
	4, 4, 4 },
	{ 1, 1, 0, 0, 2, 1, 2,
	  1, 1, 0, 0, 2, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0, 	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2 },
	spell_refresh,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		FALSE,	SLOT(81),	12,	18,	0,
	"refresh",		"!Refresh!",		"",NULL,NULL
    },

    {
	"remove curse",
	{ 102, 35, 102, 102, 93, 49, 65,
	   93, 25, 102, 102, 83, 39, 55,
	83, 83, 83,		15, 15, 15,
	102, 102, 102,	102, 102, 102,
	73, 73, 73,		29, 29, 29,
	45, 45, 45 },
	{ 0, 1, 0, 0, 2, 1, 2,
	  1, 1, 0, 0, 2, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2 },
	spell_remove_curse,	TAR_OBJ_CHAR_DEF,	POS_STANDING,
	NULL,		FALSE,	SLOT(35),	 5,	12,	0,
	"",			"!Remove Curse!",	"",NULL,NULL
    },

    {
	"restore mana",
	{ 52, 39, 103, 103, 58, 40, 103,
	  42, 29, 103, 103, 47, 30, 103,
	32, 32, 32,		19, 19, 19,
	103, 103, 103,	103, 103, 103,
	37, 37, 37,		20, 20, 20,
	103, 103, 103 },
	{ 8, 0, 0, 0, 0, 0, 0,
	  7, 0, 0, 0, 0, 0, 0,
	6, 6, 6,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
	spell_restore_mana,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		FALSE,	SLOT(251),	1,	18,	0,
	"restore mana",		"!Restore Mana!",	"",NULL,NULL
    },

    {
	"resurrect",
	{ 103, 103, 103, 103, 103, 103, 28,
	  103, 103, 103, 103, 103, 103, 18,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	8, 8, 8 },
	{ 0, 0, 0, 0, 0, 0, 1,
	  0, 0, 0, 0, 0, 0, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	1, 1, 1 },
	spell_resurrect,	TAR_IGNORE,		POS_STANDING,
	NULL,		TRUE,	SLOT(235),	 35,	12,	0,
	"",			"!Resurrect!",	"",NULL,NULL
    },

    {
        "globe of invulnerability",
	{ 52, 39, 103, 103, 58, 40, 103,
	  42, 29, 103, 103, 47, 30, 103,
	32, 32, 32,		19, 19, 19,
	103, 103, 103,	103, 103, 103,
	37, 37, 37,		20, 20, 20,
	103, 103, 103 },
	{ 1, 1, 0, 0, 2, 1, 0,
	  1, 1, 0, 0, 2, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	0, 0, 0 },
        spell_sanctuary,        TAR_CHAR_SELF,     POS_STANDING,
        &gsn_sanctuary, FALSE,  SLOT(36),       85,     12,	0,
        "",                     "The white globe around your body fades.",
	"",NULL,NULL
    },

    {
	"sanctuary",
	{ 52, 39, 103, 103, 58, 40, 103,
	  42, 29, 103, 103, 47, 30, 103,
	32, 32, 32,		19, 19, 19,
	103, 103, 103,	103, 103, 103,
	37, 37, 37,		20, 20, 20,
	103, 103, 103 },
	{ 1, 1, 0, 0, 2, 1, 0,
	  1, 1, 0, 0, 2, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	0, 0, 0 },
        spell_room_sanctuary,        TAR_IGNORE,     POS_STANDING,
        &gsn_room_sanctuary, FALSE,  SLOT(0),       200,     24,	0,
        "",              "You feel ready to protect your surroundings again.",
        "You feel less safe in this room.\n\r",NULL,NULL
    },

    {
        "darkshield",
	{ 52, 39, 103, 103, 58, 40, 103,
	  42, 29, 103, 103, 47, 30, 103,
	32, 32, 32,		19, 19, 19,
	103, 103, 103,	103, 103, 103,
	37, 37, 37,		20, 20, 20,
	103, 103, 103 },
	{ 1, 1, 0, 0, 2, 1, 0,
	  1, 1, 0, 0, 2, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	0, 0, 0 },
        spell_sanctuary,        TAR_CHAR_SELF,     POS_STANDING,
        &gsn_darkshield, FALSE,  SLOT(36),       85,     12,	0,
        "",                     "The dark shield surrounding your body fades.",
	"",NULL,NULL
    },

	{
	"sense vitality",
	{
          100, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103,  25, 103
        },
	{
            1,   1,   0,   0,   2,   1,  1,
	    1,   1,   0,   0,   2,   1,  1,
	    1,   1,   1,   1,   1,   1,  0,
            0,   0,   0,   0,   0,   2,  2,
            2,   1,   1,   1,	1,   1,  1
        },
	spell_sense_vitality,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_sense,		TRUE,	SLOT(157),	50,	12,	0,
	"{rvitality link{x",		"{rYou no longer sense a second pulse{x",	"",NULL,NULL
    },
    
    {
	"shield",
	{ 40, 40, 102, 60, 58, 55, 102,
	  30, 30, 102, 50, 48, 45, 102,
	20, 20, 20,		20, 20, 20,
	102, 102, 102,	40, 40, 40,
	38, 38, 38,		35, 35, 35,
	102, 102, 102 },
	{ 1, 1, 0, 2, 2, 1, 0,
	  1, 1, 0, 2, 2, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	2, 2, 2,
	2, 2, 2,	1, 1, 1,
	0, 0, 0 },
	spell_shield,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		FALSE,	SLOT(67),	12,	18,	0,
	"",			"Your force shield shimmers then fades away.",
	"",NULL,NULL
    },

    {
	"shocking grasp",
	{ 20, 103, 103, 103, 103, 30, 27,
	  10, 103, 103, 103, 103, 20, 17,
	6, 6, 6,		103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	10, 10, 10,
	8, 8, 8 },
	{ 1, 0, 0, 0, 0, 1, 1,
	  1, 0, 0, 0, 0, 1, 1,
	1, 1, 1,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_shocking_grasp,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(53),	15,	12,	0,
	"shocking grasp",	"!Shocking Grasp!",	"",NULL,NULL
    },

    {
	"shockshield",
	{ 60, 40, 102, 102, 102, 90, 102,
	  50, 30, 102, 102, 102, 80, 102,
	40, 40, 40,		20, 20, 20,
	102, 102, 102,	102, 102, 102,
	102, 102, 102,	70, 70, 70,
	102, 102, 102 },
	{ 3, 3, 0, 0, 0, 3, 0,
	  3, 3, 0, 0, 0, 3, 0,
	3, 3, 3,	3, 3, 3,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	3, 3, 3,
	0, 0, 0 },
	spell_shockshield,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		FALSE,	SLOT(412),	75,	12,	0,
	"lightning bolt",	"Your crackling shield sizzles and fades.",
	"",NULL,NULL
    },

    {
	"sleep",
	{ 20, 25, 103, 103, 102, 28, 15,
	  10, 15, 103, 103, 102, 18, 5,
	7, 7, 7,		10, 10, 10,
	103, 103, 103,	103, 103, 103,
	102, 102, 102,	11, 11, 11,
	3, 3, 3 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_sleep,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_sleep,	TRUE,	SLOT(38),	15,	12,	0,
	"",			"You feel less tired.",	"",NULL,NULL
    },

    {
        "slow",
	{ 45, 40, 103, 103, 60, 66, 51,
	  35, 30, 103, 103, 50, 56, 41,
	25, 25, 25,		20, 20, 20,
	103, 103, 103,	103, 103, 103,
	40, 40, 40,		46, 46, 46,
	31, 31, 31 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
        spell_slow,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,		TRUE,	SLOT(515),      30,     12,	0,
        "",                     "You feel yourself speed up.",	"",NULL,NULL
    },


    {
	"stone skin",
	{ 49, 60, 102, 102, 70, 70, 102,
	  39, 50, 102, 102, 60, 60, 102,
	29, 29, 29,		40, 40, 40,
	102, 102, 102,	102, 102, 102,
	50, 50, 50,		50, 50, 50,
	102, 102, 102 },
	{ 1, 1, 0, 0, 2, 1, 0,
	  1, 1, 0, 0, 2, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2 },
	spell_stone_skin,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		FALSE,	SLOT(66),	12,	18,	0,
	"",			"Your skin feels soft again.",	"",NULL,NULL
    },

    {
	"holy aura",
	{ 49, 60, 102, 102, 70, 70, 102,
	  39, 50, 102, 102, 60, 60, 102,
	29, 29, 29,		40, 40, 40,
	102, 102, 102,	102, 102, 102,
	50, 50, 50,		50, 50, 50,
	102, 102, 102 },
	{ 1, 1, 0, 0, 2, 1, 0,
	  1, 1, 0, 0, 2, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2 },
	spell_holy_aura,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		FALSE,	SLOT(66),	12,	18,	0,
	"",			"The aura surrounding you fades.",	"",NULL,NULL
    },

    {
	"barrier",
	{ 49, 60, 102, 102, 70, 70, 102,
	  39, 50, 102, 102, 60, 60, 102,
	29, 29, 29,		40, 40, 40,
	102, 102, 102,	102, 102, 102,
	50, 50, 50,		50, 50, 50,
	102, 102, 102 },
	{ 1, 1, 0, 0, 2, 1, 0,
	  1, 1, 0, 0, 2, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2 },
	spell_barrier,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		FALSE,	SLOT(66),	50,	18,	0,
	"",			"The barrier around you fades.",	"",NULL,NULL
    },

{
        "madness",
        { 49, 60, 102, 102, 70, 70, 102,
          39, 50, 102, 102, 60, 60, 102,
        29, 29, 29,             40, 40, 40,
        102, 102, 102,  102, 102, 102,
        50, 50, 50,             50, 50, 50,
        102, 102, 102 },
        { 1, 1, 0, 0, 2, 1, 0,
          1, 1, 0, 0, 2, 1, 2,
        1, 1, 1,        1, 1, 1,
        0, 0, 0,        0, 0, 0,
        2, 2, 2,        1, 1, 1,
        2, 2, 2 },
        spell_madness,        TAR_CHAR_SELF,          POS_STANDING,
        NULL,           FALSE,  SLOT(113),       12,     18,     0,
        "","The madness subsides.",NULL,NULL
       },


 {
        "divine intervention",
        { 49, 60, 102, 102, 70, 70, 102,
          39, 50, 102, 102, 60, 60, 102,
        29, 29, 29,             40, 40, 40,
        102, 102, 102,  102, 102, 102,
        50, 50, 50,             50, 50, 50,
        102, 102, 102 },
        { 1, 1, 0, 0, 2, 1, 0,
          1, 1, 0, 0, 2, 1, 2,
        1, 1, 1,        1, 1, 1,
        0, 0, 0,        0, 0, 0,
        2, 2, 2,        1, 1, 1,
        2, 2, 2 },
        spell_interven,        TAR_CHAR_SELF,          POS_STANDING,
        NULL,           FALSE,  SLOT(79),       12,     18,     0,
        "","",NULL,NULL
       },

{
        "acidward",
        { 49, 60, 102, 102, 70, 70, 102,
          39, 50, 102, 102, 60, 60, 102,
        29, 29, 29,             40, 40, 40,
        102, 102, 102,  102, 102, 102,
        50, 50, 50,             50, 50, 50,
        102, 102, 102 },
        { 1, 1, 0, 0, 2, 1, 0,
          1, 1, 0, 0, 2, 1, 2,
        1, 1, 1,        1, 1, 1,
        0, 0, 0,        0, 0, 0,
        2, 2, 2,        1, 1, 1,
        2, 2, 2 },
        spell_acidward,        TAR_CHAR_DEFENSIVE,          POS_STANDING,
        NULL,           FALSE,  SLOT(72),       4,     18,     0,
        "","",NULL,NULL
       },


{
        "iceward",
        { 49, 60, 102, 102, 70, 70, 102,
          39, 50, 102, 102, 60, 60, 102,
        29, 29, 29,             40, 40, 40,
        102, 102, 102,  102, 102, 102,
        50, 50, 50,             50, 50, 50,
        102, 102, 102 },
        { 1, 1, 0, 0, 2, 1, 0,
          1, 1, 0, 0, 2, 1, 2,
        1, 1, 1,        1, 1, 1,
        0, 0, 0,        0, 0, 0,
        2, 2, 2,        1, 1, 1,
        2, 2, 2 },
        spell_iceward,        TAR_CHAR_DEFENSIVE,          POS_STANDING,
        NULL,           FALSE,  SLOT(71),       4,     18,     0,
        "","",NULL,NULL
       },



 {
        "fireward",
        { 49, 60, 102, 102, 70, 70, 102,
          39, 50, 102, 102, 60, 60, 102,
        29, 29, 29,             40, 40, 40,
        102, 102, 102,  102, 102, 102,
        50, 50, 50,             50, 50, 50,
        102, 102, 102 },
        { 1, 1, 0, 0, 2, 1, 0,
          1, 1, 0, 0, 2, 1, 2,
        1, 1, 1,        1, 1, 1,
        0, 0, 0,        0, 0, 0,
        2, 2, 2,        1, 1, 1,
        2, 2, 2 },
        spell_fireward,        TAR_CHAR_DEFENSIVE,          POS_STANDING,
        NULL,           FALSE,  SLOT(76),       4,     18,     0,
        "","",NULL,NULL
       },
    {
        "generations",
        { 49, 60, 102, 102, 70, 70, 102,
          39, 50, 102, 102, 60, 60, 102,
        29, 29, 29,             40, 40, 40,
        102, 102, 102,  102, 102, 102,
        50, 50, 50,             50, 50, 50,
        102, 102, 102 },
        { 1, 1, 0, 0, 2, 1, 0,
          1, 1, 0, 0, 2, 1, 2,
        1, 1, 1,        1, 1, 1,
        0, 0, 0,        0, 0, 0,
        2, 2, 2,        1, 1, 1,
        2, 2, 2 },
        spell_generations,        TAR_CHAR_SELF,          POS_STANDING,
        NULL,           FALSE,  SLOT(66),       12,     18,	0,
        "",                     "Your knowledge begins to fade.",      "",NULL,NULL
    },

    {
        "steel flesh",
	{ 49, 60, 102, 102, 70, 70, 102,
	  39, 50, 102, 102, 60, 60, 102,
	29, 29, 29,		40, 40, 40,
	102, 102, 102,	102, 102, 102,
	50, 50, 50,		50, 50, 50,
	102, 102, 102 },
	{ 1, 1, 0, 0, 2, 1, 0,
	  1, 1, 0, 0, 2, 1, 2,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2 },
        spell_steel_flesh,       TAR_CHAR_DEFENSIVE,          POS_STANDING,
	NULL,		FALSE,	SLOT(66),	12,	18,	0,
        "",                     "Your skin loses its metallic luster.",  "",NULL,NULL
    },


    {
	"summon",
	{ 48, 23, 102, 102, 33, 58, 51,
	  38, 13, 102, 102, 23, 48, 41,
	28, 28, 28,		8, 8, 8,
	102, 102, 102,	102, 102, 102,
	13, 13, 13,		38, 38, 38,
	31, 31, 31 },
	{ 1, 1, 0, 0, 2, 1, 1,
	  1, 1, 0, 0, 2, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1 },
	spell_summon,		TAR_IGNORE,		POS_STANDING,
	NULL,		FALSE,	SLOT(40),	50,	12,	0,
	"",			"!Summon!",		"",NULL,NULL
    },


    {
	"teleport",
	{ 24, 44, 102, 102, 59, 102, 59,
	  14, 34, 102, 102, 49, 102, 49,
	10, 10, 10,		24, 24, 24,
	102, 102, 102,	102, 102, 102,
	39, 39, 39,		102, 102, 102,
	39, 39, 39 },
	{ 1, 1, 0, 0, 2, 0, 1,
	  1, 1, 0, 0, 2, 0, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
	spell_teleport,		TAR_CHAR_SELF,		POS_FIGHTING,
	NULL,	 	TRUE,	SLOT( 2),	35,	12,	0,
	"",			"!Teleport!",		"",NULL,NULL
    },

    {
  	"torrents of flame",
	{ 38, 44, 103, 103, 55, 44, 10,
	  28, 34, 103, 103, 45, 34, 1,
	18, 18, 18,		24, 24, 24,
	103, 103, 103,	103, 103, 103,
	35, 35, 35,		24, 24, 24,
	1, 1, 1 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
    spell_torrents_flame,  	TAR_CHAR_OFFENSIVE,    POS_FIGHTING,
	NULL,       	TRUE,	SLOT(758),	75,	16,	0,
        "torrents of flame",	"the smoke leaves your eyes.",		"",NULL,NULL
    },

    {
	"transport",
	{ 25, 46, 102, 102, 45, 53, 51,
	  15, 36, 102, 102, 35, 43, 41,
	10, 10, 10,		26, 26, 26,
	102, 102, 102,	102, 102, 102,
	25, 25, 25,		33, 33, 33,
	31, 31, 31 },
	{ 1, 1, 0, 0, 2, 1, 1,
	  1, 1, 0, 0, 2, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1 },
	spell_transport,	TAR_OBJ_TRAN,		POS_STANDING,
	NULL,		FALSE,	SLOT(524),	30,	12,	0,
	"",			"!Transport!",		"",NULL,NULL
    },

    {
	"unite",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 93,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 7,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
	spell_unite,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,		TRUE,	SLOT(540),	5,	12,	0,
	"united charge",			"!Unite!",		"",NULL,NULL
    },

    {
	"ventriloquate",
	{ 1, 102, 102, 102, 5, 5, 4,
	  1, 102, 102, 102, 1, 1, 3,
	1, 1, 1,		102, 102, 102,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	2, 2, 2 },
	{ 1, 0, 0, 0, 2, 1, 1,
	  1, 0, 0, 0, 2, 1, 1,
	1, 1, 1,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1 },
	spell_ventriloquate,	TAR_IGNORE,		POS_STANDING,
	NULL,		TRUE,	SLOT(41),	 5,	12,	0,
	"",			"!Ventriloquate!",	"",NULL,NULL
    },

    {
	"voodoo",
	{ 102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	102, 102, 102,	30, 102, 102,
	102, 102, 102,	102, 102, 102,
	102, 102, 102,	102, 102, 102,
	18, 18, 18 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	2, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
	spell_voodoo,		TAR_IGNORE,		POS_STANDING,
	NULL,		TRUE,	SLOT(286),	80,	12,	0,
	"",			"!Voodoo!",		"",NULL,NULL
    },

    {
	"weaken",
	{ 21, 28, 103, 103, 40, 31, 25,
	  11, 18, 103, 103, 30, 21, 15,
	8, 8, 8,		11, 11, 11,
	103, 103, 103,	103, 103, 103,
	20, 20, 20,		11, 11, 11,
	10, 10, 10 },
	{ 1, 1, 0, 0, 0, 1, 1,
	  1, 1, 0, 0, 0, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	1, 1, 1 },
	spell_weaken,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(68),	20,	12,	0,
	"spell",		"You feel stronger.",	"",NULL,NULL
    },

	{
    "wizard eye",
        { 70, 73, 103, 103, 95, 75, 103,
	  60, 63, 103, 103, 85, 65, 103,
	50, 50, 50,		53, 53, 53,
	103, 103, 103,	103, 103, 103,
	75, 75, 75,		55, 55, 55,
	103, 103, 103 },
	{ 1, 1, 0, 0, 3, 1, 0,
	  1, 1, 0, 0, 2, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	0, 0, 0 },
    spell_wizard_eye,    TAR_IGNORE,             POS_STANDING,
    NULL,      TRUE,         SLOT(603),      60,     18,	0,
    "wizard eye",        "!Wizard Eye!", "",NULL,NULL
    },

    {
	"word of recall",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
	spell_word_of_recall,	TAR_CHAR_SELF,		POS_RESTING,
	NULL,		TRUE,	SLOT(42),	 5,	12,	0,
	"",			"!Word of Recall!",	"",NULL,NULL
    },

/*
 * Dragon breath
 */
    {
	"acid breath",
	{ 62, 103, 103, 103, 103, 103, 103,
	  52, 103, 103, 103, 103, 103, 103,
	42, 42, 42,		103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103 },
	{ 1, 0, 0, 0, 0, 0, 0,
	  1, 0, 0, 0, 0, 0, 0,
	1, 1, 1,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
	spell_acid_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(200),	100,	14,	0,
	"blast of acid",	"!Acid Breath!",	"",NULL,NULL
    },

    {
	"fire breath",
	{ 80, 103, 103, 103, 103, 103, 103,
	  70, 103, 103, 103, 103, 103, 103,
	60, 60, 60,		103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103 },
          { 1, 0, 0, 0, 0, 0, 0,
	    1, 0, 0, 0, 0, 0, 0,
	1, 1, 1,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
	spell_fire_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(201),	100,	18,	0,
	"blast of flame",	"The smoke leaves your eyes.",	"",NULL,NULL
    },

    {
	"frost breath",
	{ 67, 103, 103, 103, 103, 103, 103,
	  57, 103, 103, 103, 103, 103, 103,
	47, 47, 47,		103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103 },
	{ 1, 0, 0, 0, 0, 0, 0,
	  1, 0, 0, 0, 0, 0, 0,
	1, 1, 1,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
	spell_frost_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(202),	100,	14,	0,
	"blast of frost",	"!Frost Breath!",	"",NULL,NULL
    },

    {
	"gas breath",
	{ 77, 103, 103, 103, 103, 103, 103,
	  67, 103, 103, 103, 103, 103, 103,
	57, 57, 57,		103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103 },
	{ 1, 0, 0, 0, 0, 0, 0,
	  1, 0, 0, 0, 0, 0, 0,
	1, 1, 1,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
	spell_gas_breath,	TAR_IGNORE,		POS_FIGHTING,
	NULL,		TRUE,	SLOT(203),	100,	14,	0,
	"blast of gas",		"!Gas Breath!",		"",NULL,NULL
    },

    {
	"lightning breath",
	{ 74, 103, 103, 103, 103, 103, 103,
	  64, 103, 103, 103, 103, 103, 103,
	54, 54, 54,		103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103 },
	{ 1, 0, 0, 0, 0, 0, 0,
	  1, 0, 0, 0, 2, 0, 0,
	1, 1, 1,	0, 0, 0,
	0, 0, 0,	0, 0,	0,
	2, 2, 2,	0, 0, 0,
	0, 0, 0 },
	spell_lightning_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		TRUE,	SLOT(204),	150,	18,	0,
	"blast of lightning",	"!Lightning Breath!",	"",NULL,NULL
    },

/*
 * Spells for mega1.are from Glop/Erkenbrand.
 */
    {
        "general purpose",
	{ 104, 104, 104, 104, 104, 104, 104,
	  104, 104, 104, 104, 104, 104, 104,
	104, 104, 104,	104, 104, 104,
	104, 104, 104,	104, 104, 104,
	104, 104, 104,	104, 104, 104,
	104, 104, 104 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_general_purpose,  TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,		TRUE,	SLOT(401),      0,      12,	0,
        "general purpose ammo", "!General Purpose Ammo!",	"",NULL,NULL
    },

    {
        "high explosive",
	{ 104, 104, 104, 104, 104, 104, 104,
	  104, 104, 104, 104, 104, 104, 104,
	104, 104, 104,	104, 104, 104,
	104, 104, 104,	104, 104, 104,
	104, 104, 104,	104, 104, 104,
	104, 104, 104 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_high_explosive,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,		TRUE,	SLOT(402),      25,      12,	0,
        "high explosive ammo",  "!High Explosive Ammo!",	"",NULL,NULL
    },

  {
	"stomp",

	{103, 103, 103, 103, 75, 103, 103,
	 103, 103, 103, 103, 70, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	65, 65, 65,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 5, 0, 0,
	  0, 0, 0, 0, 4, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	3, 3, 3,	0, 0, 0,
	0, 0, 0 },
	spell_stomp, 		TAR_IGNORE,		POS_FIGHTING,
	NULL,		TRUE, 	SLOT(611),	65,	12,	0,
	"stomp",		"!Stomps!",		"",NULL,NULL
    },


/* combat and weapons skills */


    {
	"axe",
	{ 102, 2, 1, 1, 1, 1, 1,
	  102, 1, 1, 1, 1, 1, 1,
	102, 102, 102,	1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 6, 5, 3, 4, 6, 5,
	  0, 5, 5, 2, 3, 5, 4,
	0, 0, 0,	4, 4, 4,
	4, 4, 4,	2, 2, 2,
	3, 3, 3,	4, 4, 4,
	4, 4, 4 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_axe,	TRUE,	SLOT( 0),       0,      0,	0,
        "",                     "!Axe!",		"",NULL,NULL
    },

    {
        "dagger",
	{ 1, 1, 1, 1, 1, 1, 1,
	  1, 1, 1, 1, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	{ 2, 3, 2, 1, 2, 3, 1,
	  2, 2, 1, 1, 1, 2, 1,
	2, 2, 2,	2, 2, 2,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dagger,	TRUE,	SLOT( 0),       0,      0,	0,
        "",                     "!Dagger!",		"",NULL,NULL
    },

    {
	"flail",
	{ 102, 2, 1, 1, 1, 1, 1,
	  102, 1, 1, 1, 1, 1, 1,
	102, 102, 102,	1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 3, 6, 3, 4, 3, 5,
	  0, 2, 5, 2, 3, 2, 4,
	0, 0, 0,	2, 2, 2,
	4, 4, 4,	2, 2, 2,
	3, 3, 3,	2, 2, 2,
	3, 3, 3 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_flail,	TRUE,	SLOT( 0),       0,      0,	0,
        "",                     "!Flail!",		"",NULL,NULL
    },

    {
	"mace",
	{ 102, 1, 1, 1, 1, 1, 1,
	  102, 1, 1, 1, 1, 1, 1,
	102, 102, 102,	1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 1, 3, 2, 3, 2, 3,
	  0, 1, 3, 1, 2, 1, 2,
	0, 0, 0,	1, 1, 1,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	1, 1, 1,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_mace,	TRUE,	SLOT( 0),       0,      0,	0,
        "",                     "!Mace!",		"",NULL,NULL
    },

    {
	"polearm",
	{ 102, 1, 1, 1, 1, 1, 1,
	  102, 1, 1, 1, 1, 1, 1,
	102, 102, 102,	1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 6, 6, 3, 3, 2, 5,
	  0, 5, 5, 2, 2, 1, 4,
	0, 0, 0,	4, 4, 4,
	4, 4, 4,	2, 2, 2,
	2, 2, 2,	1, 1, 1,
	3, 3, 3 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_polearm,	TRUE,	SLOT( 0),       0,      0,	0,
        "",                     "!Polearm!",		"",NULL,NULL
    },

    {
	"shield block",
	{ 25, 1, 1, 1, 1, 1, 1,
	  15, 1, 1, 1, 1, 1, 1,
	10, 10, 10,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 6, 4, 6, 1, 2, 4, 3,
	  5, 3, 5, 1, 1, 3, 2,
	4, 4, 4,		2, 2, 2,
	4, 4, 4,		1, 1, 1,
	1, 1, 1,		2, 2, 2,
	2, 2, 2 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_shield_block,	TRUE,	SLOT(0),	0,	0,	0,
	"",			"!Shield!",		"",NULL,NULL
    },

    {
	"spear",
	{ 102, 1, 1, 1, 1, 1, 1,
	  102, 1, 1, 1, 1, 1, 1,
	102, 102, 102,	1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 4, 4, 4, 2, 1, 3, 5,
	  3, 3, 3, 1, 1, 2, 4,
	2, 2, 2,	2, 2, 2,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	3, 3, 3 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_spear,	TRUE,	SLOT( 0),       0,      0,	0,
        "",                     "!Spear!",		"",NULL,NULL
    },

    {
	"sword",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_sword,	TRUE,	SLOT( 0),       0,      0,	0,
        "",                     "!sword!",		"",NULL,NULL
    },

    {
	"whip",
	{ 1, 1, 1, 1, 1, 1, 1,
	  1, 1, 1, 1, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	{ 6, 5, 5, 3, 3, 4, 5,
	  5, 4, 4, 3, 2, 3, 4,
	4, 4, 4,	3, 3, 3,
	3, 3, 3,	2, 2, 2,
	2, 2, 2,	2, 2, 2,
	3, 3, 3 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_whip,	TRUE,	SLOT( 0),       0,      0,	0,
        "",                     "!Whip!",	"",NULL,NULL
    },

	{
	    "ambush",
	{ 103, 103, 103, 103, 63, 103, 103,
	  103, 103, 103, 103, 53, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	43, 43, 43,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 5, 0, 0,
	  0, 0, 0, 0, 4, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	3, 3, 3,	0, 0, 0,
	0, 0, 0 },
	    spell_null,             TAR_IGNORE,             POS_FIGHTING,
          &gsn_ambush,        TRUE,   SLOT( 0),        0,     12,	2,
	    "ambush",             "!Ambush!",		"",NULL,NULL
	},


    {
        "backstab",
	{ 103, 103, 1, 102, 102, 103, 15,
	  103, 103, 1, 102, 102, 103, 5,
	103, 103, 103,	103, 103, 103,
	1, 1, 1,		102, 102, 102,
	102, 102, 102,	103, 103, 103,
	3, 3, 3 },
	{ 0, 0, 2, 0, 0, 0, 3,
	  0, 0, 2, 0, 0, 0, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_backstab,	TRUE,	SLOT( 0),        0,     12,	2,
        "backstab",             "!Backstab!",		"",NULL,NULL
    },

    {
	"bash",
	{ 103, 103, 103, 1, 103, 103, 103,
	  103, 103, 103, 1, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	1, 1, 1,
	103, 103, 103,	103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 2, 0, 0, 0,
	  0, 0, 0, 1, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_bash,	TRUE,	SLOT( 0),       0,      12,	2,
        "bash",                 "!Bash!",		"",NULL,NULL
    },

    {
	"berserk",
	{ 103, 103, 103, 27, 37, 103, 103,
	  103, 103, 103, 17, 27, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	11, 11, 11,
	17, 17, 17,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 2, 3, 0, 0,
	  0, 0, 0, 1, 2, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_berserk,	TRUE,	SLOT( 0),       0,      24,	0,
        "",                     "You feel your pulse slow down.",	"",NULL,NULL
    },

    {
        "blind fighting",
	{ 103, 103, 103, 25, 35, 103, 103,
	  103, 103, 103, 10, 25, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	7, 7, 7,
	15, 15, 15,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 2, 3, 0, 0,
	  0, 0, 0, 2, 3, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	2, 2, 2,
	3, 3, 3,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_blind_fighting,TRUE,SLOT( 0),        0,     0,	0,
        "",                     "!Blindfight!",		"",NULL,NULL
    },

    {
	"butcher",
	{ 103, 103, 10, 10, 3, 2, 103,
	  103, 103, 2, 2, 1, 1, 5,
	103, 103, 103,	103, 103, 103,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	103, 103, 103 },
	{ 0, 0, 2, 2, 1, 1, 0,
	  0, 0, 1, 1, 1, 1, 2,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_butcher,	TRUE,	SLOT( 0),       0,      12,	0,
        "",                     "!Butcher!",	"",NULL,NULL
    },


    {
        "circle",
	{ 103, 103, 38, 103, 103, 103, 103,
	  103, 103, 26, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	16, 16, 16,		103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 4, 0, 0, 0, 0,
	  0, 0, 3, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_circle,	TRUE,	SLOT( 0),        0,     24,	2,
        "circle",             "!Circle!",		"",NULL,NULL
    },

    {
	"dirt kicking",
	{ 103, 103, 4, 4, 1, 9, 3,
	  103, 103, 1, 1, 1, 8, 2,
	103, 103, 103,	103, 103, 103,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		6, 6, 6,
	2, 2, 2 },
	{ 0, 0, 4, 4, 4, 6, 2,
	  0, 0, 3, 3, 3, 5, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	2, 2, 2,
	2, 2, 2,	4, 4, 4,
	1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_dirt,	TRUE,	SLOT( 0),	0,	12,	2,
	"kicked dirt",		"You rub the dirt out of your eyes.",	"",NULL,NULL
    },

    {
        "disarm",
	{ 103, 103, 18, 18, 32, 103, 20,
	  103, 103, 8, 8, 22, 103, 10,
	103, 103, 103,	103, 103, 103,
	6, 6, 6,		6, 6, 6,
	12, 12, 12,		103, 103, 103,
	8, 8, 8 },
	{ 0, 0, 6, 3, 5, 0, 3,
	  0, 0, 5, 2, 4, 0, 2,
	0, 0, 0,	0, 0, 0,
	4, 4, 4,	2, 2, 2,
	3, 3, 3,	0, 0, 0,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_disarm,	TRUE,	SLOT( 0),        0,     12,	2,
        "",                     "!Disarm!",		"",NULL,NULL
    },

    {
        "dodge",
     { 1, 1, 1, 19, 10, 103, 10,
       1, 1, 1, 9, 1, 103, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		7, 7, 7,
	1, 1, 1,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 6, 5, 0, 2,
	  0, 0, 3, 5, 4, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	4, 4, 4,
	3, 3, 3,	0, 0, 0,
	1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dodge,	TRUE,	SLOT( 0),        0,     0,	0,
        "bypass",                     "!Dodge!",              "",NULL,NULL
    },

    {
        "dual wield",
	{ 103, 45, 35, 3, 29, 103, 40,
	  103, 35, 8, 1, 15, 103, 30,
	103, 103, 103,	25, 25, 25,
	6, 6, 6,		1, 1, 1,
	10, 10, 10,		103, 103, 103,
	20, 20, 20 },
	{ 0, 6, 3, 2, 3, 0, 4,
	  0, 5, 2, 1, 3, 0, 3,
	0, 0, 0,		4, 4, 4,
	2, 2, 2,		1, 1, 1,
	2, 2, 2,		0, 0, 0,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dual_wield,	TRUE,	SLOT( 0),        0,     0,	0,
        "",			"!Dual Wield!",		"",NULL,NULL
    },

    {
        "enhanced damage",
	{ 103, 103, 103, 1, 30, 103, 103,
	  100, 103, 103, 1, 20, 103, 103,
	90, 90, 90,		103, 103, 103,
	103, 103, 103,	1, 1, 1,
	10, 10, 10,		103, 103, 103,
	103, 103, 103 },
	{ 0, 6, 6, 3, 6, 6, 6,
	  0, 5, 5, 2, 5, 5, 5,
	0, 0, 0,	4, 4, 4,
	4, 4, 4,	2, 2, 2,
	4, 4, 4,	4, 4, 4,
	4, 4, 4 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_enhanced_damage,   TRUE,	SLOT( 0),        0,     0,	0,
        "",                     "!Enhanced Damage!",	"",NULL,NULL
    },

    {
	"envenom",
	{ 103, 103, 15, 90, 19, 103, 9,
	  103, 103, 5, 80, 9, 103, 1,
	103, 103, 103,	103, 103, 103,
	3, 3, 3,		70, 70, 70,
	7, 7, 7,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 6, 1, 0, 3,
	  0, 0, 3, 5, 1, 0, 2,
	0, 0, 0,		0, 0, 0,
	2, 2, 2,		4, 4, 4,
	1, 1, 1,		0, 0, 0,
	2, 2, 2 },
	spell_null,		TAR_IGNORE,	  	POS_RESTING,
	&gsn_envenom,	TRUE,	SLOT(0),	0,	20,	0,
	"",			"!Envenom!",		"",NULL,NULL
    },

    {
        "fear",
     { 102, 102, 102, 102, 102, 102, 102,
       102, 102, 102, 102, 102, 102, 102,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_fear,             TAR_CHAR_OFFENSIVE,             POS_STANDING,
        NULL,     TRUE,   SLOT(841),      100,      6,	0,
        "fear",          "You no longer feel paranoid.",              "",NULL,NULL
    },

    {
        "fear aura",
	{ 103, 103, 103, 103, 103, 103, 29,
	  103, 103, 103, 103, 103, 103, 11,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	9, 9, 9 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fear,	TRUE,	SLOT( 0),	10,	16,	0,
        "fear",             "You feel brave again.",		"",NULL,NULL
    },

    {
        "feed",
	{ 103, 103, 103, 103, 103, 103, 29,
	  103, 103, 103, 103, 103, 103, 11,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	9, 9, 9 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_feed,	TRUE,	SLOT( 0),	0,	12,	2,
        "feed",             "!Feed!",		"",NULL,NULL
    },

    {
	"gouge",
	{ 103, 103, 52, 103, 103, 103, 103,
	  103, 103, 24, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	14, 14, 14,		103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 2, 0, 0, 0, 0,
	  0, 0, 1, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	1, 1, 1,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_gouge,	TRUE,	SLOT( 0),	0,	12,	2,
	"gouge",		"Your vision clears.",	"",NULL,NULL
    },

    {
        "grip",
	{ 103, 103, 103, 25, 35, 103, 103,
	  103, 103, 103, 10, 25, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	7, 7, 7,
	15, 15, 15,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 2, 3, 0, 0,
	  0, 0, 0, 1, 2, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_grip,	TRUE,	SLOT( 0),        0,     0,	0,
        "",                     "!Grip!",		"",NULL,NULL
    },

    {
	"hand to hand",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_hand_to_hand,	TRUE,	SLOT( 0),	0,	0,	0,
	"",			"!Hand to Hand!",	"",NULL,NULL
    },

    {
        "kick",
	{ 103, 18, 21, 12, 20, 17, 15,
	  103, 8, 11, 2, 10, 7, 5,
	103, 103, 103,	6, 6, 6,
	7, 7, 7,		1, 1, 1,
	6, 6, 6,		5, 5, 5,
	3, 3, 3 },
	{ 0, 4, 6, 3, 4, 6, 2,
	  0, 3, 5, 2, 3, 5, 1,
	0, 0, 0,	2, 2, 2,
	4, 4, 4,	2, 2, 2,
	2, 2, 2,	4, 4, 4,
	1, 1, 1 },
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_kick,	TRUE,	SLOT( 0),        0,     12,	0,
        "kick",                 "!Kick!",		"",NULL,NULL
    },

    {
        "riposte",
	{ 103, 18, 21, 12, 20, 17, 15,
	  103, 8, 11, 2, 10, 7, 5,
	103, 103, 103,	6, 6, 6,
	7, 7, 7,		1, 1, 1,
	6, 6, 6,		5, 5, 5,
	3, 3, 3 },
	{ 0, 4, 6, 3, 4, 6, 2,
	  0, 3, 5, 2, 3, 5, 1,
	0, 0, 0,	2, 2, 2,
	4, 4, 4,	2, 2, 2,
	2, 2, 2,	4, 4, 4,
	1, 1, 1 },
        spell_null,             TAR_IGNORE,     POS_FIGHTING,
        &gsn_riposte,   TRUE,   SLOT( 0),        0,     6,	0,
        "riposte",                 "!Riposte!",		"",NULL,NULL
    },

    {
        "acute vision",
	{ 103, 18, 21, 12, 20, 17, 15,
	  103, 8, 11, 2, 10, 7, 5,
	103, 103, 103,	6, 6, 6,
	7, 7, 7,		1, 1, 1,
	6, 6, 6,		5, 5, 5,
	3, 3, 3 },
	{ 0, 4, 6, 3, 4, 6, 2,
	  0, 3, 5, 2, 3, 5, 1,
	0, 0, 0,	2, 2, 2,
	4, 4, 4,	2, 2, 2,
	2, 2, 2,	4, 4, 4,
	1, 1, 1 },
        spell_null,             TAR_IGNORE,     POS_RESTING,
        &gsn_acute_vision,	TRUE,	SLOT( 0),        0,     0,	0,
        "",                 "Your eyes become less Focused",		"",NULL,NULL
    },

    {
        "skin",
	{ 103, 18, 21, 12, 20, 17, 15,
	  103, 8, 11, 2, 10, 7, 5,
	103, 103, 103,	6, 6, 6,
	7, 7, 7,		1, 1, 1,
	6, 6, 6,		5, 5, 5,
	3, 3, 3 },
	{ 0, 4, 6, 3, 4, 6, 2,
	  0, 3, 5, 2, 3, 5, 1,
	0, 0, 0,	2, 2, 2,
	4, 4, 4,	2, 2, 2,
	2, 2, 2,	4, 4, 4,
	1, 1, 1 },
        spell_null,             TAR_IGNORE,     POS_STANDING,
        &gsn_skin,	TRUE,	SLOT( 0),        0,     24,	0,
        "skin",                 "!Skin!",		"",NULL,NULL
    },

    {
        "demand",
	{ 103, 18, 21, 12, 20, 17, 15,
	  103, 8, 11, 2, 10, 7, 5,
	103, 103, 103,	6, 6, 6,
	7, 7, 7,		1, 1, 1,
	6, 6, 6,		5, 5, 5,
	3, 3, 3 },
	{ 0, 4, 6, 3, 4, 6, 2,
	  0, 3, 5, 2, 3, 5, 1,
	0, 0, 0,	2, 2, 2,
	4, 4, 4,	2, 2, 2,
	2, 2, 2,	4, 4, 4,
	1, 1, 1 },
        spell_null,             TAR_IGNORE,     POS_STANDING,
        &gsn_demand,	TRUE,	SLOT( 0),        0,     12,	0,
        "",                 "!Demand!",		"",NULL,NULL
    },

    {
        "assassinate",
        { 103, 18, 21, 12, 20, 17, 15,
          103, 8, 11, 2, 10, 7, 5,
        103, 103, 103,  6, 6, 6,
        7, 7, 7,                1, 1, 1,
        6, 6, 6,                5, 5, 5,
        3, 3, 3 },
        { 0, 4, 6, 3, 4, 6, 2,
          0, 3, 5, 2, 3, 5, 1,
        0, 0, 0,        2, 2, 2,
        4, 4, 4,        2, 2, 2,
        2, 2, 2,        4, 4, 4,
        1, 1, 1 },
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_STANDING,
        &gsn_assassinate,      TRUE,   SLOT( 0),        0,     0,	0,
        "assassinate",                 "!Assassinate!",               "",NULL,NULL
    },

    {
        "strangle",
        { 103, 18, 21, 12, 20, 17, 15,
          103, 8, 11, 2, 10, 7, 5,
        103, 103, 103,  6, 6, 6,
        7, 7, 7,                1, 1, 1,
        6, 6, 6,                5, 5, 5,
        3, 3, 3 },
        { 0, 4, 6, 3, 4, 6, 2,
          0, 3, 5, 2, 3, 5, 1,
        0, 0, 0,        2, 2, 2,
        4, 4, 4,        2, 2, 2,
        2, 2, 2,        4, 4, 4,
        1, 1, 1 },
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_STANDING,
        &gsn_strangle,      TRUE,   SLOT( 0),        0,     12,	0,
        "Strangle",                 "!Strangle!",               "",NULL,NULL
    },

    {
        "smokebomb",
        { 103, 18, 21, 12, 20, 17, 15,
          103, 8, 11, 2, 10, 7, 5,
        103, 103, 103,  6, 6, 6,
        7, 7, 7,                1, 1, 1,
        6, 6, 6,                5, 5, 5,
        3, 3, 3 },
        { 0, 4, 6, 3, 4, 6, 2,
          0, 3, 5, 2, 3, 5, 1,
        0, 0, 0,        2, 2, 2,
        4, 4, 4,        2, 2, 2,
        2, 2, 2,        4, 4, 4,
        1, 1, 1 },
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_smokebomb,      TRUE,   SLOT( 0),        0,     16,	0,
        "smokebomb",                 "Your eyes clear of the smoke.",
        "The smoke flows out of the room.\n\r",NULL,NULL
    },

    {
        "layhands",
        { 103, 18, 21, 12, 20, 17, 15,
          103, 8, 11, 2, 10, 7, 5,
        103, 103, 103,  6, 6, 6,
        7, 7, 7,                1, 1, 1,
        6, 6, 6,                5, 5, 5,
        3, 3, 3 },
        { 0, 4, 6, 3, 4, 6, 2,
          0, 3, 5, 2, 3, 5, 1,
        0, 0, 0,        2, 2, 2,
        4, 4, 4,        2, 2, 2,
        2, 2, 2,        4, 4, 4,
        1, 1, 1 },
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_RESTING,
        &gsn_layhands,      TRUE,   SLOT( 0),        0,     12,	5,
        "layhands",                 "!LayOnHands!",               "",NULL,NULL
    },

    {
        "lead",
        { 103, 18, 21, 12, 20, 17, 15,
          103, 8, 11, 2, 10, 7, 5,
        103, 103, 103,  6, 6, 6,
        7, 7, 7,                1, 1, 1,
        6, 6, 6,                5, 5, 5,
        3, 3, 3 },
        { 0, 4, 6, 3, 4, 6, 2,
          0, 3, 5, 2, 3, 5, 1,
        0, 0, 0,        2, 2, 2,
        4, 4, 4,        2, 2, 2,
        2, 2, 2,        4, 4, 4,
        1, 1, 1 },
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_RESTING,
        &gsn_lead,      TRUE,   SLOT( 0),        0,     12,	5,
        "lead",                 "You feel the rush of battle fading from your limbs.",               "",NULL,NULL
    },

    {
        "coup de coup",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,     POS_FIGHTING,
        &gsn_coup_de_coup,	TRUE,	SLOT( 0),        0,     0,	0,
        "coup de coup",                 "!CoupDeCoup!",		"",NULL,NULL
    },

    {
        "parry",
	{ 103, 103, 19, 1, 15, 27, 17,
	  103, 103, 9, 1, 5, 17, 7,
	103, 103, 103,	103, 103, 103,
	7, 7, 7,		1, 1, 1,
	3, 3, 3,		11, 11, 11,
	5, 5, 5 },
	{ 0, 0, 4, 4, 5, 5, 2,
	  0, 0, 3, 3, 4, 4, 1,
	0, 0, 0,		0, 0, 0,
	2, 2, 2,		2, 2, 2,
	3, 3, 3,		3, 3, 3,
	1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_parry,	TRUE,	SLOT( 0),        0,     0,	0,
        "counter",                     "!Parry!",              "",NULL,NULL
    },

    {
        "rescue",
	{ 103, 103, 103, 1, 13, 103, 103,
	  103, 103, 103, 1, 3, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	1, 1, 1,
	2, 2, 2,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 4, 3, 0, 0,
	  0, 0, 0, 3, 2, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	2, 2, 2,
	2, 2, 2,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_rescue,	TRUE,	SLOT( 0),        0,     12,	0,
        "",                     "!Rescue!",		"",NULL,NULL
    },

    {
	"trip",
	{ 103, 103, 2, 22, 25, 103, 5,
	  103, 103, 1, 12, 15, 103, 1,
	103, 103, 103,	103, 103, 103,
	1, 1, 1,		9, 9, 9,
	10, 10, 10,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 8, 5, 0, 3,
	  0, 0, 3, 7, 4, 0, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	6, 6, 6,
	3, 3, 3,	0, 0, 0,
	2, 2, 2 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_trip,	TRUE,	SLOT( 0),	0,	12,	0,
	"trip",			"!Trip!",		"",NULL,NULL
    },

    {
        "stun",
	{ 103, 103, 103, 45, 103, 103, 103,
	  103, 103, 103, 23, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	13, 13, 13,
	103, 103, 103,	103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 3, 0, 0, 0,
	  0, 0, 0, 2, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	2, 2, 2,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_stun,	TRUE,	SLOT( 0),        0,     0,	0,
        "",			"!Stun!",		"",NULL,NULL
    },

    {
        "second attack",
	{ 45, 36, 18, 7, 1, 25, 20,
	  35, 26, 8, 6, 1, 15, 10,
	25, 25, 25,		16, 16, 16,
	6, 6, 6,		4, 4, 4,
	1, 1, 1,		10, 10, 10,
	8, 8, 8 },
	{ 7, 8, 5, 3, 4, 8, 2,
	  6, 7, 4, 2, 3, 7, 1,
	5, 5, 5,	6, 6, 6,
	3, 3, 3,	2, 2, 2,
	2, 2, 2,	6, 6, 6,
	1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_second_attack,	TRUE,	SLOT( 0),        0,     0,	0,
        "",                     "!Second Attack!",	"",NULL,NULL
    },

    {
        "third attack",
	{ 53, 53, 36, 18, 29, 80, 30,
	  43, 43, 26, 8, 19, 70, 20,
	33, 33, 33,		33, 33, 33,
	16, 16, 16,		6, 6, 6,
	12, 12, 12,		60, 60, 60,
	15, 15, 15 },
	{ 6, 6, 9, 4, 7, 7, 3,
	  5, 5, 8, 3, 6, 6, 2,
	4, 4, 4,	4, 4, 4,
	7, 7, 7,	2, 2, 2,
	5, 5, 5,	5, 5, 5,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_third_attack,	TRUE,	SLOT( 0),        0,     0,	0,
        "",                     "!Third Attack!",	"",NULL,NULL
    },

    {
        "multistrike",
        { 53, 53, 36, 18, 29, 80, 30,
          43, 43, 26, 8, 19, 70, 20,
        33, 33, 33,             33, 33, 33,
        16, 16, 16,             6, 6, 6,
        12, 12, 12,             60, 60, 60,
        15, 15, 15 },
        { 6, 6, 9, 4, 7, 7, 3,
          5, 5, 8, 3, 6, 6, 2,
        4, 4, 4,        4, 4, 4,
        7, 7, 7,        2, 2, 2,
        5, 5, 5,        5, 5, 5,
        2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_multistrike,      TRUE,   SLOT( 0),        0,     0,	0,
        "berserker rage",                "!MultiStrike!",       "",NULL,NULL
    },

    {
	"fourth attack",
     { 103, 103, 90, 65, 80, 103, 103,
       103, 103, 80, 55, 70, 103, 103,
	103, 103, 103,	103, 103, 103,
	70, 70, 70,		45, 45, 45,
	60, 60, 60,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 10, 4, 5, 0, 0,
	  0, 0, 9, 3, 4, 0, 0,
	0, 0, 0,	0, 0, 0,
	8, 8, 8,	2, 2, 2,
	3, 3, 3,	0, 0, 0,
	0, 0, 0 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_fourth_attack,	TRUE,	SLOT(0),	0,	0,	0,
	"",			"!Fourth Attack!",	"",NULL,NULL
    },

    {
	"fifth attack",
	{ 103, 103, 103, 80, 102, 103, 103,
	  103, 103, 103, 70, 102, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	60, 60, 60,
	102, 102, 102,	103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 4, 0, 0, 0,
	  0, 0, 0, 3, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	2, 2, 2,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_fifth_attack,	TRUE,	SLOT(0),	0,	0,	0,
	"",			"!Fifth Attack!",	"",NULL,NULL
    },


/* non-combat skills */

    {
            "sharpen",
            { 103, 103, 18, 19, 28, 36, 103,
              103, 103, 11, 15, 26, 34, 103,
	103, 103, 103,	103, 103, 103,
	9, 9, 9,		10, 10, 10,
	16, 16, 16,		24, 24, 24,
	103, 103, 103 },
            { 0, 0, 4, 4, 5, 5, 0,
              0, 0, 3, 3, 4, 5, 0,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	2, 2, 2,
	3, 3, 3,	4, 4, 4,
	0, 0, 0 },
            spell_null,             TAR_IGNORE,             POS_RESTING,
       &gsn_sharpen,   TRUE,        SLOT(0),        0,      15,	0,
            "",                     "!Sharpen!",            "",NULL,NULL
    },

    {
	"fast healing",
	{ 102, 13, 12, 9, 55, 34, 2,
	  102, 12, 10, 1, 45, 24, 1,
	102, 102, 102,	9, 9, 9,
	7, 7, 7,		1, 1, 1,
	35, 35, 35,		14, 14, 14,
	1, 1, 1 },
     { 5, 5, 5, 4, 8, 5, 1,
       4, 4, 4, 3, 7, 4, 1,
	3, 3, 3,	3, 3, 3,
	3, 3, 3,	2, 2, 2,
	6, 6, 6,	3, 3, 3,
	1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_fast_healing,	TRUE,	SLOT( 0),	0,	0,	0,
	"",			"!Fast Healing!",	"",NULL,NULL
    },

    {
	"haggle",
	{ 50, 60, 1, 55, 42, 80, 65,
	  40, 50, 1, 45, 32, 70, 55,
	30, 30, 30,		40, 40, 40,
	1, 1, 1,		35, 35, 35,
	22, 22, 22,		60, 60, 60,
	45, 45, 45 },
	{ 3, 4, 3, 6, 5, 5, 4,
	  2, 3, 2, 5, 4, 4, 3,
	2, 2, 2,	2, 2, 2,
	2, 2, 2,	4, 4, 4,
	3, 3, 3,	3, 3, 3,
	2, 2, 2 },
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_haggle,	TRUE,	SLOT( 0),	0,	0,	0,
	"",			"!Haggle!",		"",NULL,NULL
    },

    {
	"hide",
	{ 102, 102, 1, 18, 5, 102, 1,
	  102, 102, 1, 8, 1, 102, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		6, 6, 6,
	1, 1, 1,		102, 102, 102,
	1, 1, 1 },
	{ 0, 0, 4, 6, 5, 0, 3,
	  0, 0, 3, 5, 4, 0, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	4, 4, 4,
	3, 3, 3,	0, 0, 0,
	2, 2, 2 },
	spell_null,		TAR_IGNORE,		POS_RESTING,
        &gsn_hide,      TRUE,   SLOT( 999),        0,     12,	0,
	"",			"!Hide!",		"",NULL,NULL
    },

    {
	"lore",
	{ 15, 15, 9, 25, 5, 14, 7,
	  5, 5, 1, 15, 4, 4, 6,
	3, 3, 3,	3, 3, 3,
	1, 1, 1,	10, 10, 10,
	2, 2, 2,	2, 2, 2,
	4, 4, 4 },
	{ 6, 6, 4, 7, 4, 5, 4,
	  5, 5, 3, 6, 3, 4, 3,
	4, 4, 4,	4, 4, 4,
	2, 2, 2,	5, 5, 5,
	2, 2, 2,	3, 3, 3,
	2, 2, 2 },
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_lore,	TRUE,	SLOT( 0),	0,	36,	0,
	"",			"!Lore!",		"",NULL,NULL
    },

    {
	"meditation",
	{ 9, 9, 102, 102, 15, 13, 80,
	  8, 1, 102, 102, 5, 3, 70,
	6, 6, 6,		1, 1, 1,
	102, 102, 102,	102, 102, 102,
	3, 3, 3,		2, 2, 2,
	60, 60, 60 },
	{ 5, 5, 0, 0, 6, 6, 7,
	  4, 4, 0, 0, 5, 5, 6,
	3, 3, 3,	3, 3, 3,
	0, 0, 0,	0, 0, 0,
	4, 4, 4,	4, 4, 4,
	5, 5, 5 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_meditation,	TRUE,	SLOT( 0),	0,	0,	0,
	"",			"You come out of your meditation.",		"",NULL,NULL
    },

    {
	"peek",
	{ 102, 102, 1, 102, 102, 102, 4,
	  90, 102, 1, 102, 102, 102, 1,
	80, 80, 80,		102, 102, 102,
	1, 1, 1,		102, 102, 102,
	102, 102, 102,	102, 102, 102,
	1, 1, 1 },
	{ 0, 0, 3, 0, 0, 0, 4,
	  6, 0, 2, 0, 0, 0, 3,
	5, 5, 5,	0, 0, 0,
	2, 2, 2,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	2, 2, 2 },
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_peek,	TRUE,	SLOT( 0),	 0,	 0,	0,
	"",			"!Peek!",		"",NULL,NULL
    },

    {
	"pick lock",
	{ 102, 102, 10, 102, 102, 102, 37,
	  102, 102, 6, 102, 102, 102, 27,
	102, 102, 102,	102, 102, 102,
	4, 4, 4,		102, 102, 102,
	102, 102, 102,	102, 102, 102,
	17, 17, 17 },
	{ 0, 0, 4, 0, 0, 0, 8,
	  0, 0, 3, 0, 0, 0, 7,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	6, 6, 6 },
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_pick_lock,	TRUE,	SLOT( 0),	 0,	12,	0,
	"",			"!Pick!",		"",NULL,NULL
    },

    {
	"sneak",
	{ 102, 102, 6, 15, 2, 102, 2,
	  102, 102, 5, 5, 1, 102, 1,
	102, 102, 102,	102, 102, 102,
	4, 4, 4,		3, 3, 3,
	1, 1, 1,		102, 102, 102,
	1, 1, 1 },
	{ 0, 0, 4, 6, 3, 0, 1,
	  0, 0, 3, 5, 2, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	4, 4, 4,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_STANDING,
        &gsn_sneak,     TRUE,   SLOT( 999),        0,     12,	0,
	"",			"You no longer feel stealthy.",	"",NULL,NULL
    },
{
	"gods favor",
	{ 102, 102, 102, 102, 102, 102, 102,
	  102, 102, 102, 102, 102, 102, 102,
	102, 102, 102,	102, 102, 102,
	102, 102, 102,		102, 102, 102,
	102, 102, 102,		102, 102, 102,
	102, 102, 102 },
	{ 0, 0, 4, 6, 3, 0, 1,
	  0, 0, 3, 5, 2, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	4, 4, 4,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_STANDING,
        &gsn_god_favor,     TRUE,   SLOT( 999),        0,     0,	0,
	"",	"The favor of your god has left you.",	"",NULL,NULL
    },

    {
	"steal",
	{ 102, 102, 7, 102, 102, 102, 102,
	  102, 102, 5, 102, 102, 102, 102,
	102, 102, 102,	102, 102, 102,
	3, 3, 3,		102, 102, 102,
	102, 102, 102,	102, 102, 102,
	102, 102, 102 },
	{ 0, 0, 4, 0, 0, 0, 0,
	  0, 0, 3, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_steal,	TRUE,	SLOT( 0),	 0,	24,	0,
	"",			"!Steal!",		"",NULL,NULL
    },

    {
	"scrolls",
	{ 2, 1, 1, 1, 1, 1, 1,
	  1, 1, 1, 1, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	{ 2, 3, 5, 8, 7, 3, 8,
	  1, 2, 4, 7, 6, 2, 7,
	1, 1, 1, 	2, 2, 2,
	3, 3, 3,	6, 6, 6,
	5, 5, 5,	2, 2, 2,
	6, 6, 6 },
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_scrolls,	TRUE,	SLOT( 0),	0,	24,	0,
	"",			"!Scrolls!",		"",NULL,NULL
    },

    {
	"staves",
	{ 1, 1, 1, 1, 1, 1, 1,
	  1, 1, 1, 1, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	{ 2, 3, 5, 8, 7, 3, 8,
	  1, 2, 4, 7, 6, 2, 7,
	1, 1, 1,	2, 2, 2,
	3, 3, 3,	6, 6, 6,
	5, 5, 5,	2, 2, 2,
	6, 6, 6 },
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_staves,	TRUE,	SLOT( 0),	0,	12,	0,
	"",			"!Staves!",		"",NULL,NULL
    },

    {
	"track",
	{ 102, 102, 45, 102, 20, 102, 102,
	  102, 102, 35, 102, 10, 102, 102,
	102, 102, 102,	102, 102, 102,
	25, 25, 25,		102, 102, 102,
	102, 102, 102,	102, 102, 102,
	102, 102, 102 },
	{ 0, 0, 5, 0, 2, 0, 0,
	  0, 0, 4, 0, 1, 0, 0,
	0, 0, 0,	0, 0, 0,
	3, 3, 3,	0, 0, 0,
	1, 1, 1,	0, 0, 0,
	0, 0, 0 },
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_track,	TRUE,	SLOT( 0),	 0,	12,	0,
	"",			"!Track!",		"",NULL,NULL
    },

    {
	"wands",
	{ 1, 1, 1, 1, 1, 1, 1,
	  1, 1, 1, 1, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	{ 2, 3, 5, 8, 7, 3, 8,
	  1, 2, 5, 7, 6, 2, 7,
	1, 1, 1,	2, 2, 2,
	4, 4, 4, 	6, 6, 6,
	5, 5, 5,	2, 2, 2,
	6, 6, 6 },
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_wands,	TRUE,	SLOT( 0),	0,	12,	0,
	"",			"!Wands!",		"",NULL,NULL
    },

    {
	"recall",
	{ 1, 1, 1, 1, 1, 1, 1,
	  1, 1, 1, 1, 1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	{ 2, 2, 2, 2, 2, 2, 2,
	  1, 1, 2, 1, 1, 1, 1,
	1, 1, 1, 	1, 1, 1,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_recall,	TRUE,	SLOT( 0),	0,	12,	0,
	"",			"!Recall!",		"",NULL,NULL
    },

    {
        "sixth attack",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,     POS_FIGHTING,
      &gsn_sixth_attack,      TRUE,   SLOT( 0),        0,     0,	0,
        "",                 "!SixthAttack!",         "",NULL,NULL
    },

    {
        "divine protection",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_divine_protection,     TAR_CHAR_DEFENSIVE, POS_STANDING,
        &gsn_divine_protection,      TRUE,   SLOT( 0),        100,     24,	0,
        "",                 "You feel less protected by your diety.",         "",NULL,NULL
    },

    {
        "protective aura",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_protective_aura,     TAR_CHAR_DEFENSIVE,     POS_STANDING,
        &gsn_protective_aura,      TRUE,   SLOT( 0),        400,     16,	0,
        "",                 "The golden aura around you fades.",         "",NULL,NULL
    },

    {
        "turn undead",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_turn_undead,        TAR_CHAR_OFFENSIVE,     POS_STANDING,
        &gsn_turn_undead,      TRUE,   SLOT( 0),        150,     12,	2,
        "holy intonations",                 "!TurnUndead!",         "",NULL,NULL
    },

    {
        "constance",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_hold_align,             TAR_CHAR_DEFENSIVE,     POS_STANDING,
        &gsn_hold_align,      TRUE,   SLOT( 0),        175,     12,	0,
        "",                 "You feel ready to pick sides again.",         "",NULL,NULL
    },

    {
        "wrath of god",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_wrath_of_god,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_wrath_of_god,      TRUE,   SLOT( 0),        50,     16,	0,
        "divine wrath",                 "!WrathOfGod!",         "",NULL,NULL
    },

    {
        "quickening",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,     POS_STANDING,
        &gsn_quickening,      TRUE,   SLOT(999),        150,     12,	0,
        "{5q{ruickening{x",                 "!Quickening!",         "",NULL,NULL
    },

    {
        "decapitate",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,     POS_FIGHTING,
        &gsn_decapitate,      TRUE,   SLOT( 0),        100,     12,	0,
        "{gd{cecapitation{x",                 "You feel ready again to swing your blade.",         "",NULL,NULL
    },

    {
        "blade mastery",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,            POS_FIGHTING,
        &gsn_blademastery,      TRUE,   SLOT( 0),        50,     6,	0,
        "{rd{Readly {bb{glow{x",                 "!BladeMastery!",         "",NULL,NULL
    },

    {
	"shriek",
	{ 103, 103, 2, 22, 25, 103, 5,
	  103, 103, 1, 12, 15, 103, 1,
	103, 103, 103,	103, 103, 103,
	1, 1, 1,		9, 9, 9,
	10, 10, 10,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 8, 5, 0, 3,
	  0, 0, 3, 7, 4, 0, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	6, 6, 6,
	3, 3, 3,	0, 0, 0,
	2, 2, 2 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
        &gsn_shriek,    TRUE,   SLOT( 0),       0,      12,	0,
	"shriek",		"Your ears stop ringing.",	"",NULL,NULL
    },

    {
        "burning skin",
        { 103, 103, 103, 103, 103, 103, 103,
          103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
        103, 103, 103,                103, 103, 103,
        103, 103, 103,             103, 103, 103,
        103, 103, 103 },
	{ 0, 0, 4, 8, 5, 0, 3,
	  0, 0, 3, 7, 4, 0, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	6, 6, 6,
	3, 3, 3,	0, 0, 0,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_DEAD,
        &gsn_on_fire,    TRUE,   SLOT( 0),       0,      0,	0,
        "burn",               "The fire on you burns out.",      "",NULL,NULL
    },

    {
        "twirl",
        { 103, 103, 103, 103, 103, 103, 103,
          103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
        103, 103, 103,             103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_twirl,    TRUE,   SLOT( 0),        0,     27,	0,
        "blade twirl",             "!Twirl!",               "",NULL,NULL
    },

    {
        "forestwalk",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
	102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_forestwalk,             TAR_IGNORE,             POS_FIGHTING,
        NULL,           FALSE,  SLOT(0),       80,     12,	0,
        "",                     "!FWalk!",               "",NULL,NULL
    },

    {
        "shadowgate",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
	102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_shadowwalk,             TAR_IGNORE,             POS_FIGHTING,
        NULL,           FALSE,  SLOT(0),       80,     12,	0,
        "",                     "!SWalk!",               "",NULL,NULL
    },

    {
        "blend",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
	102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_forest_blend,      FALSE,  SLOT(0),       80,     12,	0,
        "",                     "You step out into the open.",               "",NULL,NULL
    },

    {
        "herb",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
	102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_herb,           FALSE,  SLOT(0),       80,     12,	0,
        "",                     "The herbs wear off.",               "",NULL,NULL
    },

    {
        "detect terrain",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
	102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_detect_terrain,    FALSE,  SLOT(0),       80,     12,	0,
        "",                     "The environment goes out of focus.",               "",NULL,NULL
    },

    {
        "crevice",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
		102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
		0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
		0, 0, 0,	0, 0, 0,
		0, 0, 0 },
        spell_crevice,             TAR_CHAR_OFFENSIVE,             POS_FIGHTING,
        NULL,           FALSE,  SLOT(0),       75,     12,	0,
        "crevice",                     "!Crevice!",               "",NULL,NULL
    },

    {
        "entangle",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
	102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_entangle,             TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        NULL,           TRUE, SLOT(0),       50,     12,	0,
        "entangle",                     "!Entangle!",               "",NULL,NULL
    },

    {
        "barkskin",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
	102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_barkskin,           FALSE,  SLOT(0),       80,     12,	0,
        "",                     "Your skin regains its smoothness.",               "",NULL,NULL
    },


    {

     "phase",
     { 1, 1, 1, 19, 10, 103, 10,
       1, 1, 1, 9, 1, 103, 1,
       1, 1, 1,                1, 1, 1,
       1, 1, 1,                7, 7, 7,
       1, 1, 1,                103, 103, 103,
       1, 1, 1 },
       { 0, 0, 4, 6, 5, 0, 2,
         0, 0, 3, 5, 4, 0, 1,
      0, 0, 0,        0, 0, 0,
       2, 2, 2,        4, 4, 4,
       3, 3, 3,        0, 0, 0,
       1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_phase,    TRUE,   SLOT( 0),        0,     0,	0,
        "",                     "!Phase!",             "",NULL,NULL
    },

    {
        "charm song",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
	102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        song_charm_person,      TAR_CHAR_OFFENSIVE, POS_STANDING,
        NULL,           FALSE,  SLOT(0),       10,     18,	0,
        "",                     "You feel more independent!",               "",NULL,NULL
    },

    {
        "calming song",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
	102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        song_calm,             TAR_IGNORE,             POS_FIGHTING,
        NULL,           FALSE,  SLOT(0),       10,     16,	0,
        "",                     "You feel less calm.",               "",NULL,NULL
    },

   {
        "rage",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102,  102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
        0, 0, 0,        0, 0, 0,
        0, 0, 0,        0, 0, 0,
        0, 0, 0,        0, 0, 0,
        0, 0, 0 },
        song_rage,             TAR_CHAR_OFFENSIVE,             POS_FIGHTING,
        NULL,           FALSE,  SLOT(0),       15,     14,      0,
        "rage song",                     "!rage!",               "",NULL,NULL
    },


    {
        "sonic blast",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
	102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        song_sonic_blast,             TAR_CHAR_OFFENSIVE,             POS_FIGHTING,
        NULL,           FALSE,  SLOT(0),       15,     14,	0,
        "sonic blast",                     "!sonicblast!",               "",NULL,NULL
    },

    {
        "channel",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
	102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_channel,           TAR_IGNORE,             POS_STANDING,
        &gsn_channel,           FALSE,  SLOT(0),       100,     16,	0,
        "channeling",               "The energy field around you has depleted.",               "",NULL,NULL
    },

    {
        "wither",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
	102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_wither,             TAR_CHAR_OFFENSIVE,             POS_FIGHTING,
        &gsn_wither,           FALSE,  SLOT(0),       40,     12,	0,
        "withering",                     "You feel more complete.",               "",NULL,NULL
    },

    {
        "soulbind",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
	102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_soulbind,             TAR_CHAR_OFFENSIVE,             POS_FIGHTING,
        &gsn_soulbind,           FALSE,  SLOT(0),       75,     12,	0,
        "",                     "You feel ready to soulbind once again.",               "",NULL,NULL
    },

    {
        "domineer",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
	102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_domineer,             TAR_CHAR_OFFENSIVE,             POS_STANDING,
        &gsn_domineer,           FALSE,  SLOT(0),       80,     12,	0,
        "",                     "You no longer feel controlled.",               "",NULL,NULL
    },

    {
        "headache",
        { 102, 102, 102, 102, 102, 102, 102,
          102, 102, 102, 102, 102, 102, 102,
        102, 102, 102,             102, 102, 102,
	102, 102, 102,	102, 102, 102,
        102, 102, 102,             102, 102, 102,
        102, 102, 102 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
        0, 0, 0,        0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_headache,             TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        &gsn_headache,           FALSE,  SLOT(0),       50,     12,	0,
        "",                     "Your head feels less congested.",               "",NULL,NULL
    },

    {
        "dimension walk",
	{ 54, 34, 102, 102, 42, 61, 56,
	  44, 24, 102, 102, 32, 51, 46,
	34, 34, 34,		14, 14, 14,
	102, 102, 102,	102, 102, 102,
	22, 22, 22,		41, 41, 41,
	36, 36, 36 },
	{ 1, 1, 0, 0, 2, 1, 1,
	  1, 1, 0, 0, 2, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1 },
        spell_dimension_walk,     TAR_IGNORE,             POS_FIGHTING,
        NULL,           FALSE,  SLOT(0),       80,     12,	0,
        "",                     "!Dimension Walk!",               "",NULL,NULL
    },

    {
        "earthmeld",
	{ 9, 37, 103, 102, 12, 39, 13,
	  8, 27, 103, 102,  2, 29,  3,
	6, 6, 6,		17, 17, 17,
	103, 103, 103,	102, 102, 102,
	1, 1, 1,		19, 19, 19,
	2, 2, 2 },
	{ 1, 1, 0, 0, 2, 1, 1,
	  1, 1, 0, 0, 2, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1 },
        spell_earthmeld,            TAR_OBJ_CHAR_DEF,       POS_STANDING,
        &gsn_earthmeld,     FALSE,  SLOT(0),        15,     12,	0,
        "",                     "You rise from the earth.",
        "",NULL,NULL
    },

    {
        "call to arms",
	{ 9, 37, 103, 102, 12, 39, 13,
	  8, 27, 103, 102,  2, 29,  3,
	6, 6, 6,		17, 17, 17,
	99, 99, 99,	99, 99, 99,
	1, 1, 1,		19, 19, 19,
	2, 2, 2 },
	{ 1, 1, 0, 0, 2, 1, 1,
	  1, 1, 0, 0, 2, 1, 1,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1 },
        spell_enforcer,            TAR_IGNORE,       POS_FIGHTING,
        NULL,     FALSE,  SLOT(999),        275,     12,	0,
        "",                     "You may summon more enforcers.",
        "",NULL,NULL
    },

    {
        "open palm technique",
	{ 103, 103, 103, 1, 30, 103, 103,
	  100, 103, 103, 1, 20, 103, 103,
	90, 90, 90,		103, 103, 103,
	103, 103, 103,	1, 1, 1,
	10, 10, 10,		103, 103, 103,
	103, 103, 103 },
	{ 0, 6, 6, 3, 6, 6, 6,
	  0, 5, 5, 2, 5, 5, 5,
	0, 0, 0,	4, 4, 4,
	4, 4, 4,	2, 2, 2,
	4, 4, 4,	4, 4, 4,
	4, 4, 4 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_palm,   TRUE,   SLOT(999),        0,     0,	0,
        "",                     "Your palms relax.",    "",NULL,NULL
    },

    {
        "martial arts",
	{ 103, 103, 103, 1, 30, 103, 103,
	  100, 103, 103, 1, 20, 103, 103,
	90, 90, 90,		103, 103, 103,
	103, 103, 103,	1, 1, 1,
	10, 10, 10,		103, 103, 103,
	103, 103, 103 },
	{ 0, 6, 6, 3, 6, 6, 6,
	  0, 5, 5, 2, 5, 5, 5,
	0, 0, 0,	4, 4, 4,
	4, 4, 4,	2, 2, 2,
	4, 4, 4,	4, 4, 4,
	4, 4, 4 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_martial_arts,   TRUE,   SLOT(999),        0,     0,	0,
        "strike",                     "You no longer use martial arts techniques.",    "",NULL,NULL
    },

    {
        "animate dead",
	{ 103, 103, 103, 103, 103, 103, 45,
	  103, 103, 103, 103, 103, 103, 35,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	25, 25, 25 },
	{ 0, 0, 0, 0, 0, 0, 1,
	  0, 0, 0, 0, 0, 0, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	1, 1, 1 },
        spell_animate_dead,          TAR_OBJ_INV,            POS_STANDING,
        &gsn_animate_dead,           TRUE,   SLOT(239),       100,    16,	0,
        "",                     "Your powers over the dead return.",    "",NULL,NULL
    },

    {
        "animate skeleton",
	{ 103, 103, 103, 103, 103, 103, 45,
	  103, 103, 103, 103, 103, 103, 35,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	25, 25, 25 },
	{ 0, 0, 0, 0, 0, 0, 1,
	  0, 0, 0, 0, 0, 0, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	1, 1, 1 },
        spell_animate_skeleton,          TAR_OBJ_INV,            POS_STANDING,
        &gsn_animate_skeleton,           TRUE,   SLOT(239),       75,    16,	0,
        "",                     "Your powers over the dead return.",    "",NULL,NULL
    },

    {
        "mummify",
	{ 103, 103, 103, 103, 103, 103, 45,
	  103, 103, 103, 103, 103, 103, 35,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	25, 25, 25 },
	{ 0, 0, 0, 0, 0, 0, 1,
	  0, 0, 0, 0, 0, 0, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	1, 1, 1 },
        spell_mummify,          TAR_OBJ_INV,            POS_STANDING,
        &gsn_mummify,           TRUE,   SLOT(239),       150,    16,	0,
        "",                     "Your powers over the dead return.",    "",NULL,NULL
    },

    {
        "decay corpse",
	{ 103, 103, 103, 103, 103, 103, 45,
	  103, 103, 103, 103, 103, 103, 35,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,	103, 103, 103,
	25, 25, 25 },
	{ 0, 0, 0, 0, 0, 0, 1,
	  0, 0, 0, 0, 0, 0, 1,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 0, 0,
	1, 1, 1 },
        spell_decay_corpse,          TAR_OBJ_INV,            POS_STANDING,
        NULL,           TRUE,   SLOT(239),       50,    16,	0,
        "",                     "!decay!",    "",NULL,NULL
    },

    {
        "mana shield",
	{ 52, 39, 103, 103, 58, 40, 103,
	  42, 29, 103, 103, 47, 30, 103,
	32, 32, 32,		19, 19, 19,
	103, 103, 103,	103, 103, 103,
	37, 37, 37,		20, 20, 20,
	103, 103, 103 },
	{ 1, 1, 0, 0, 2, 1, 0,
	  1, 1, 0, 0, 2, 1, 0,
	1, 1, 1,	1, 1, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	0, 0, 0 },
        spell_mana_shield,        TAR_CHAR_SELF,     POS_STANDING,
        &gsn_mana_shield, TRUE,  SLOT(0),       2000,     36,	0,
        "",                     "Your health fades, your mind is restored.",
	"",NULL,NULL
    },

    {
        "throw",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,     POS_FIGHTING,
        &gsn_throw,      TRUE,   SLOT( 0),        20,     12,	0,
        "{gt{chrow{x",             "",         "",NULL,NULL
    },

    {
        "prayer",
        { 103, 103, 103, 103, 103, 103, 103,
          103, 103, 103, 103, 103, 103, 103,
        103, 103, 103,  103, 103, 103,
        103, 103, 103,          103, 103, 103,
        103, 103, 103,          103, 103, 103,
        103, 103, 103 },
        { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
        0, 0, 0,        0, 0, 0,
        0, 0, 0,        0, 2, 0,
        0, 0, 0,        0, 0, 0,
        0, 0, 0 },
        spell_null,             TAR_IGNORE,     POS_FIGHTING,
        &gsn_prayer,      TRUE,   SLOT( 0),        20,     16,	0,
        "{gp{crayer{x",             "",         "",NULL,NULL
    },


    {
        "lunge",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,     POS_FIGHTING,
        &gsn_lunge,      TRUE,   SLOT( 0),        100,     12,	0,
        "{gl{cunge{x",             "",         "",NULL,NULL
    },
     {
        "chop",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,     POS_FIGHTING,
        &gsn_chop,      TRUE,   SLOT( 0),        100,     12,	0,
        "{gc{chop{x",             "",         "",NULL,NULL
    },

     {
        "head cut",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,     POS_FIGHTING,
        &gsn_head_cut,      TRUE,   SLOT( 0),        100,     12,	0,
        "{gH{cead cut{x",             "",         "",NULL,NULL
    },

    {
        "tumble",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,     POS_FIGHTING,
        &gsn_roll,      TRUE,   SLOT( 0),        100,     12,	0,
        "",             "",         "",NULL,NULL
    },

    {
        "nerve",
	{ 103, 103, 103, 103, 103, 103, 103,
	  103, 103, 103, 103, 103, 103, 103,
	103, 103, 103,	103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103,		103, 103, 103,
	103, 103, 103 },
	{ 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0,	0, 2, 0,
	0, 0, 0,	0, 0, 0,
	0, 0, 0 },
        spell_null,             TAR_IGNORE,     POS_FIGHTING,
        &gsn_nerve,      TRUE,   SLOT(999),        100,     12,	0,
        "",             "",         "",NULL,NULL
    },

    {
        "punch",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
        &gsn_punch,      TRUE,   SLOT( 0),       0,      0,	0,
        "punch",                     "!Punch!",       "",NULL,NULL
    },

    {
        "elbow",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
        &gsn_elbow,      TRUE,   SLOT( 0),       0,      0,	0,
        "elbow",                     "!Elbow!",       "",NULL,NULL
    },

    {
        "knee",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
        &gsn_knee,      TRUE,   SLOT( 0),       0,      0,	0,
        "knee",                     "!Knee!",       "",NULL,NULL
    },

    {
        "headbutt",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
        &gsn_headbutt,      TRUE,   SLOT( 0),       0,      0,	0,
        "headbutt",                     "!Headbutt!",       "",NULL,NULL
    },

    {
        "spinkick",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
        &gsn_spinkick,      TRUE,   SLOT( 0),       0,      18,	0,
        "spin-kick",                     "!Spinkick!",       "",NULL,NULL
    },

    {
        "gut",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
        &gsn_gut,      TRUE,   SLOT( 0),       0,      0,	0,
        "gut-punch",                     "!Gut!",       "",NULL,NULL
    },

    {
        "weaponbash",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
        &gsn_weaponbash,      TRUE,   SLOT( 0),       0,      0,	0,
        "weapon-bash",                     "!Weaponbash!",       "",NULL,NULL
    },

    {
        "shoulder",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
        &gsn_shoulder,      TRUE,   SLOT( 0),       0,      0,	0,
        "shoulder-ram",                     "!Shoulder!",       "",NULL,NULL
    },

    {
        "hurl",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
        &gsn_hurl,      TRUE,   SLOT( 0),       0,      0,	0,
        "hurl",                     "!Hurl!",       "",NULL,NULL
    },

//hari fix bree
{
        "hari",
        { 103, 103, 22, 9, 22, 103, 12,
          103, 103, 12, 8, 12, 103, 2,
        103, 103, 103,  103, 103, 103,
        8, 8, 8,                6, 6, 6,
        8, 8, 8,                103, 103, 103,
        1, 1, 1 },
        { 0, 0, 4, 2, 4, 0, 2,
          0, 0, 3, 1, 3, 0, 1,
        0, 0, 0,        0, 0, 0,
        2, 2, 2,        1, 1, 1,
        2, 2, 2,        0, 0, 0,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_RESTING,
        &gsn_hari,      TRUE,   SLOT(999),       0,      0,	0,
        "self mutilation1",          "You feel ready to mutilate yourself again.",       "",NULL,NULL
    },


    {
        "hara kiri",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_RESTING,
        &gsn_hara_kiri,      TRUE,   SLOT(999),       0,      0,	0,
        "self mutilation",          "You feel ready to mutilate yourself again.",       "",NULL,NULL
    },

    {
        "darken blade",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
        spell_darken_blade,         TAR_OBJ_INV,             POS_STANDING,
        NULL,      TRUE,   SLOT( 0),       150,      18,	0,
        "",               "!Darken Blade!",     "$p looks less malevolant.",NULL,NULL
    },

    {
        "empower blade",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
        spell_empower_blade,         TAR_OBJ_INV,             POS_STANDING,
        NULL,      TRUE,   SLOT( 0),       150,      18,	0,
        "",               "!Empower Blade!",     "$p looses its electric charge.",NULL,NULL
    },

    {
        "flame blade",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
        spell_flame_blade,         TAR_OBJ_INV,             POS_STANDING,
        NULL,      TRUE,   SLOT( 0),       150,      18,	0,
        "",               "!Flame Blade!", "$p looses its fiery glow.",NULL,NULL
    },

    {
        "frost blade",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
        spell_frost_blade,         TAR_OBJ_INV,             POS_STANDING,
        NULL,      TRUE,   SLOT( 0),       150,      18,	0,
        "",               "!Frost Blade!", "$p warms back up.",NULL,NULL
    },

    {
        "acid blade",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
        spell_acid_blade,         TAR_OBJ_INV,             POS_STANDING,
        NULL,      TRUE,   SLOT( 0),       150,      18,	0,
        "",               "!Acid Blade!", "$p looks less corrosive.",NULL,NULL
    },

    {
        "transmute",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
        spell_transmute,         TAR_OBJ_INV,             POS_STANDING,
        NULL,      TRUE,   SLOT( 0),       2000,      24,	0,
        "",               "!Transmute",     "$p is no longer transmuted.",NULL,NULL
    },
    
    {
        "illusionary armor",
	{ 103, 103, 22, 9, 22, 103, 12,
	  103, 103, 12, 8, 12, 103, 2,
	103, 103, 103,	103, 103, 103,
	8, 8, 8,		6, 6, 6,
	8, 8, 8,		103, 103, 103,
	1, 1, 1 },
	{ 0, 0, 4, 2, 4, 0, 2,
	  0, 0, 3, 1, 3, 0, 1,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	2, 2, 2,	0, 0, 0,
	1, 1, 1 },
        spell_illusion_armor,         TAR_CHAR_SELF,             POS_STANDING,
        NULL,      TRUE,   SLOT( 0),       150,      20,	0,
        "",  "You do not feel grand anymore.", "$p looks a little less grand.",NULL,NULL
    },

    {
        "advanced sword",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_sword2,     TRUE,   SLOT( 0),       0,      0,	0,
        "",                     "!sword2!",              "",NULL,NULL
    },

   /* {
        "advanced hand to hand",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
        102, 102, 102,  102, 102, 102,
        1, 1, 1,                1, 1, 1,
        1, 1, 1,                1, 1, 1,
        1, 1, 1 },
        { 0, 0, 3, 1, 2, 4, 3,
          0, 0, 2, 1, 1, 3, 2,
        0, 0, 0,        0, 0, 0,
        2, 2, 2,        1, 1, 1,
        1, 1, 1,        2, 2, 2,
        2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_hand2,     TRUE,   SLOT( 0),       0,      0,	0,
        "",                     "!hand2!",              "",NULL,NULL
    }, */

    {
        "advanced dagger",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dagger2,     TRUE,   SLOT( 0),       0,      0,	0,
        "",                     "!dagger2!",              "",NULL,NULL
    },

    {
        "advanced spear",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_spear2,     TRUE,   SLOT( 0),       0,      0,	0,
        "",                     "!spear2!",              "",NULL,NULL
    },

     {
        "advanced hand to hand",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
        102, 102, 102,  102, 102, 102,
        1, 1, 1,                1, 1, 1,
        1, 1, 1,                1, 1, 1,
        1, 1, 1 },
        { 0, 0, 3, 1, 2, 4, 3,
          0, 0, 2, 1, 1, 3, 2,
        0, 0, 0,        0, 0, 0,
        2, 2, 2,        1, 1, 1,
        1, 1, 1,        2, 2, 2,
        2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_hand2,     TRUE,   SLOT( 0),       0,      0,	0,
        "",                     "!hand2!",              "",NULL,NULL
    },

    {
        "advanced axe",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_axe2,     TRUE,   SLOT( 0),       0,      0,	0,
        "",                     "!axe2!",              "",NULL,NULL
    },

    {
        "advanced whip",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_whip2,     TRUE,   SLOT( 0),       0,      0,	0,
        "",                     "!whip2!",              "",NULL,NULL
    },

    {
        "advanced polearm",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_polearm2,     TRUE,   SLOT( 0),       0,      0,	0,
        "",                     "!polearm2!",              "",NULL,NULL
    },

    {
        "advanced mace",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_mace2,     TRUE,   SLOT( 0),       0,      0,	0,
        "",                     "!mace2!",              "",NULL,NULL
    },

    {
        "advanced flail",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_flail2,     TRUE,   SLOT( 0),       0,      0,	0,
        "",                     "!flail2!",              "",NULL,NULL
    },

    {
        "fists of fury",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fists_fury,     TRUE,   SLOT(999),       0,      0,	0,
        "furious punch",                     "The haze of fury fades from your hands.",              "",NULL,NULL
    },

    {
        "fists of flame",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fists_flame,     TRUE,   SLOT( 0),       0,      0,	0,
        "flaming punch",          "The haze of flame fades from your hands.",              "",NULL,NULL
    },

    {
        "fists of ice",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fists_ice,     TRUE,   SLOT( 0),       0,      0,	0,
        "freezing punch",         "The haze of ice fades from your hands.",              "",NULL,NULL
    },

    {
        "fists of darkness",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fists_darkness,     TRUE,   SLOT( 0),       0,      0,	0,
        "life drain",           "The darkness lifts from your hands.",              "",NULL,NULL
    },

    {
        "fists of acid",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fists_acid,     TRUE,   SLOT( 0),       0,      0,	0,
        "acidic punch",        "Your hands stop burning.",              "",NULL,NULL
    },

    {
        "fists of liquid",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fists_liquid,     TRUE,   SLOT( 0),       0,      0,	0,
        "foaming punch",       "The liquid drips off your hands.",              "",NULL,NULL
    },

    {
        "fang fists",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fists_fang,     TRUE,   SLOT(999),       0,      0,	0,
        "venomous punch",     "The poison drips off of your hands.",              "",NULL,NULL
    },

    {
        "claw technique",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fists_claw,     TRUE,   SLOT(999),       0,      0,	0,
        "open claw",         "You remove the claws from your wrists.",              "",NULL,NULL
    },

    {
        "fists of divinity",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fists_divinity,     TRUE,   SLOT( 0),       0,      0,	0,
        "divine punch",                     "The holy haze fades from your hands.",              "",NULL,NULL
    },

    {
        "adamantium palm",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_adamantium_palm,     TRUE,   SLOT( 0),       0,      18,	0,
        "rock hard punch",      "Your muscles feel stronger again.",              "",NULL,NULL
    },

    {
        "deflect",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_deflect,     TRUE,   SLOT( 0),       0,      0,	0,
        "deflection",          "",              "",NULL,NULL
    },

    {
        "counter defense",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_counter_defense,     TRUE,   SLOT( 0),       0,      0,	0,
        "counter strike",         "",              "",NULL,NULL
    },

    {
        "roundhouse",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_roundhouse,     TRUE,   SLOT(0),       0,      12,	0,
        "roundhouse",     "",              "",NULL,NULL
    },

    {
        "spinkick",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_spinkick,     TRUE,   SLOT(0),       0,      12,	0,
        "spinkick",     "",              "",NULL,NULL
    },

     {
        "michi",
     { 102, 102, 1, 1, 1, 1,        102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_michi,     TRUE,   SLOT(999),       0,      0,	0,
        "michi",     "You lose contact with your michi.",              "",NULL,NULL
    },

{
        "inner strength",
     { 102, 102, 1, 1, 1, 1,        102, 102, 1, 1, 1, 1, 1,
        102, 102, 102,  102, 102, 102,
        1, 1, 1,                1, 1, 1,
        1, 1, 1,                1, 1, 1,
        1, 1, 1 },
        { 0, 0, 3, 1, 2, 4, 3,
          0, 0, 2, 1, 1, 3, 2,
        0, 0, 0,        0, 0, 0,
        2, 2, 2,        1, 1, 1,
        1, 1, 1,        2, 2, 2,
        2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_inner_str,     TRUE,   SLOT(999),       0,      0,	0,
        "inner strength",     "You lose contact with your inner strength.",              "",NULL,NULL
    },


    {
	"faerie fire",
	{ 12, 5, 103, 103, 103, 2, 92,
	   2, 4, 103, 103,  66, 1, 82,
	1, 1, 1,		3, 3, 3,
	103, 103, 103,	103, 103, 103,
	56, 56, 56,		1, 1, 1,
	72, 72, 72 },
	{ 1, 1, 1, 1, 1, 1, 2,
	  1, 1, 1, 1, 2, 1, 2,
	1, 1, 1,	1, 1, 1,
	1, 1, 1,	1, 1, 1,
	2, 2, 2,	1, 1, 1,
	2, 2, 2 },
	spell_faerie_fire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
        &gsn_faerie_fire,           TRUE,   SLOT(0),       25,     12,	0,
	"faerie fire",		"The pink aura around you fades away.",
	"",NULL,NULL
    },

    {
        "conceal",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_conceal,     TRUE,   SLOT(999),       0,      0,	0,
        "",     "You remove the cloak.",              "",NULL,NULL
    },

    {
        "transform",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_conceal2,     TRUE,   SLOT(999),       0,      0,	0,
        "",     "You become human again.",              "",NULL,NULL
    },

  {

      "vengeance",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_vengeance,     TRUE,   SLOT(999),       0,      0,	0,
        "",     "The divide slowly closes.",              "",NULL,NULL
    },

  {

      "shadow form",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_shadow_form,     TRUE,   SLOT(999),       0,      0,	0,
        "Shadow Form",     "The shadows subside.",              "",NULL,NULL
    },

  {

      "divide",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_divide,     TRUE,   SLOT(999),       0,      0,	0,
        "Conquering Blow",     "The divide slowly closes.",              "",NULL,NULL
    },

       {

        "vmight",
        { 10, 10, 10, 10, 10, 10, 10,
          10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1,
        1, 1, 1 },
        spell_null,            TAR_IGNORE,             POS_STANDING,
        &gsn_vmight,           TRUE,  SLOT(999),       0,   0,	0,
        "",                     "You feel the might of the Vanir flow through you!",              "",NULL,NULL
    },


 {

      "phsyco",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_phsyco,     TRUE,   SLOT(999),       0,      0,	0,
        "phsycotic rage",     "The divide slowly closes.",              "",NULL,NULL
    },

/*      {

      "omen",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_omen,     TRUE,   SLOT(999),       0,      0,	0,
        "omen",     "The omens being to fade from your mind",              "",NULL,NULL
    },
*/

    {
        "target",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_target,     TRUE,   SLOT(999),       0,      12,	0,
        "",     "You no longer target anyone.",              "",NULL,NULL
    },

    {
        "battle fury",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_battle_fury,     TRUE,   SLOT(999),       0,      24,	0,
        "",     "You feel less furious.",              "",NULL,NULL
    },

    {
        "cripple",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_cripple,     TRUE,   SLOT(999),       0,      12,	0,
        "crippling blow",     "Your bones feel complete again.",              "",&gsn_cripple_head, &gsn_cripple_leg
    },

    {
        "manuevers",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_snattack,     TRUE,   SLOT(999),       0,      12,	0,
        "manuevers",     "You begin to relax.",              "",NULL,NULL
    },


    {
        "battlecry",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_modis_anger,   TRUE,   SLOT(999),       0,      12,	0,
        "ungodly blow",     "You calm down to regain your energy.",       "",NULL,NULL
    },

    {
        "greed",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_greed,   TRUE,   SLOT(999),       0,      12,	0,
        "greedy blow",     "You calm down to regain your energy.",       "",NULL,NULL
    },


    {
        "concentration",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_concentration,   TRUE,   SLOT(999),       0,      12,	0,
        "second strike",   "You calm down to regain your stamina.",       "",NULL,NULL
    },

      {
        "critical strike",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_crit_strike, TRUE,   SLOT(241),       0,      12,	0,
        "critical strike",     "You calm down to regain your energy.",       "",NULL,NULL
    },
      /*{
        "sunder armor",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_sunder, TRUE,   SLOT(999),       0,      12,	0,
        "sunder armor",     "",       "",NULL,NULL
    },*/

     {
        "bloodlust",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_bloodlust, TRUE,   SLOT(241),       0,      12,	0,
        "adrenaline",     "Your adrenaline rush fades.",       "",NULL,NULL
    },


   {
        "vanish",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_vanish,   TRUE,   SLOT(999),       0,      12,	0,
        "",   "You calm down enough to vanish again.",       "",NULL,NULL
    },

    {
        "wander",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_wander,   TRUE,   SLOT(999),       0,      12,	0,
        "",   "You stop wandering.",       "",NULL,NULL
    },

    {
        "acute hearing",
	{ 103, 18, 21, 12, 20, 17, 15,
	  103, 8, 11, 2, 10, 7, 5,
	103, 103, 103,	6, 6, 6,
	7, 7, 7,		1, 1, 1,
	6, 6, 6,		5, 5, 5,
	3, 3, 3 },
	{ 0, 4, 6, 3, 4, 6, 2,
	  0, 3, 5, 2, 3, 5, 1,
	0, 0, 0,	2, 2, 2,
	4, 4, 4,	2, 2, 2,
	2, 2, 2,	4, 4, 4,
	1, 1, 1 },
        spell_null,             TAR_IGNORE,     POS_RESTING,
        &gsn_acute_hearing,	TRUE,	SLOT( 0),        0,     0,	0,
        "",                 "!Acute Hearing!",		"",NULL,NULL
    },
    {
        "blind faith",
     { 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,  10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 10,                10, 10, 10,
        10, 10, 1 },
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1,        1, 1, 1,
        1, 1, 1 },
        spell_null,             TAR_CHAR_OFFENSIVE,            POS_FIGHTING,
        &gsn_blind_faith,     TRUE,   SLOT(999),       0,      0,	0,
        "blind faith",     "Your bones feel complete again.",              "",NULL,NULL
    },

    {
        "blindness dust",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_blindness_dust,     TRUE,   SLOT( 0),       0,      0,	0,
        "",          "The dust clears from your eyes.",              "",NULL,NULL
    },

    {
        "follow through",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_follow_through,     TRUE,   SLOT( 0),       0,      0,	0,
        "follow through",          "",              "",NULL,NULL
    },

    {
        "ground stomp",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_ground_stomp,     TRUE,   SLOT( 0),       0,      0,	0,
        "ground stomp",          "",              "",NULL,NULL
    },

    {
        "side kick",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_sidekick,     TRUE,   SLOT( 0),       0,      12,	0,
        "kick",          "",              "",NULL,NULL
    },

    {
        "spin kick",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_spin_kick,     TRUE,   SLOT( 0),       0,      16,	0,
        "spin kick",          "",              "",NULL,NULL
    },

    {
        "healing trance",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_healing_trance,     TRUE,   SLOT( 0),       0,      0,	0,
        "",          "You come out of your trance.",              "",NULL,NULL
    },

    {
        "purify body",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_purify_body,     TRUE,   SLOT( 0),       0,      0,	0,
        "",          "Your body returns to its normal state and you feel refreshed.",              "",NULL,NULL
    },

    {
        "will of iron",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_will_of_iron,     TRUE,   SLOT( 0),       0,      0,	0,
        "",          "Your mind clears and your body relaxes.",              "",NULL,NULL
    },

    {
        "vital hit",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_vital_hit,     TRUE,   SLOT( 0),       0,      0,	0,
        "vital hit",          "",              "",NULL,NULL
    },

    {
        "quick step",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_quick_step,     TRUE,   SLOT( 0),       0,      0,	0,
        "",          "",              "",NULL,NULL
    },

    {
        "shield bash",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_shield_bash,     TRUE,   SLOT( 0),       0,      0,	0,
        "shield bash",          "",              "",NULL,NULL
    },

    {
        "cleave",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_cleave,     TRUE,   SLOT( 0),       0,      0,	0,
        "brutal cleave",          "",              "",NULL,NULL
    },

   {
        "dual wield mastery",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dual_wield2,     TRUE,   SLOT( 0),       0,      0,	0,
        "",          "",              "",NULL,NULL
    },

    {
        "embalm",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_embalm,     TRUE,   SLOT( 0),      300,      0,	0,
        "",          "The powder is all messed up and the fluid drips.",              "",NULL,NULL
    },

    {
        "flesh golem",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_lesser_golem,             TAR_OBJ_INV,             POS_STANDING,
        &gsn_lesser_golem,     TRUE,   SLOT( 0),      250,      0,	0,
        "",          "You feel ready to create another flesh golem.",              "",NULL,NULL
    },

    {
        "iron golem",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_greater_golem,           TAR_OBJ_INV,             POS_STANDING,
        &gsn_greater_golem,     TRUE,   SLOT( 0),      450,      0,	0,
        "",          "You feel ready to create another iron golem.",              "",NULL,NULL
    },

    {
        "preserve limb",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_preserve_limb,             TAR_OBJ_INV,             POS_STANDING,
        &gsn_preserve_limb,     TRUE,   SLOT( 0),      75,      0,	0,
        "",          "!Preserve Limb!",              "",NULL,NULL
    },

   {
        "cremate",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_cremate,             TAR_CHAR_OFFENSIVE,             POS_FIGHTING,
        &gsn_cremate,     TRUE,   SLOT( 0),      50,     6,	0,
        "blast of fire",          "!Cremate!",              "",NULL,NULL
    },

    {
        "imbue weapon",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_imbue_weapon,             TAR_IGNORE,             POS_STANDING,
        &gsn_imbue_weapon,     TRUE,   SLOT( 0),      400,      0,	0,
        "",          "!Imbue Weapon!",              "",NULL,NULL
    },

    {
        "restore",
     { 102, 102, 1, 1, 1, 1, 1,
       102, 102, 1, 1, 1, 1, 1,
	102, 102, 102,	102, 102, 102,
	1, 1, 1,		1, 1, 1,
	1, 1, 1,		1, 1, 1,
	1, 1, 1 },
	{ 0, 0, 3, 1, 2, 4, 3,
	  0, 0, 2, 1, 1, 3, 2,
	0, 0, 0,	0, 0, 0,
	2, 2, 2,	1, 1, 1,
	1, 1, 1,	2, 2, 2,
	2, 2, 2 },
        spell_restore,             TAR_IGNORE,             POS_STANDING,
        &gsn_restore,     TRUE,   SLOT( 0),      100,      0,	0,
        "",          "!Restore!",              "",NULL,NULL
    },

};

/* god table */
 const   struct  god_type       god_table      []           =
 {
 /*
     {
         name,
         player_good?,   player_neutral?,   player_evil?,
         player_mage?,   player_cleric?,     player_thief?, warrior?, ranger?, druid?, vampire?,
         wizard?, priest?, merc? gladiator?, strider?, sage?, lich?
     },
 */

     { "Nobody", FALSE, FALSE, FALSE,
	 TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE },

     { "Aegir",
       TRUE,      TRUE,       TRUE,
       TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE
     },

     { "Eir",
       TRUE,	TRUE,		FALSE,
       FALSE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE,
       FALSE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE,
       FALSE, FALSE, FALSE,
	 TRUE, TRUE, TRUE,
	 FALSE, FALSE, FALSE,
	 FALSE, FALSE, FALSE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE,
	 FALSE, FALSE, FALSE },

     { "Odin",
       TRUE,	TRUE, 	FALSE,
       FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, FALSE,
       FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, FALSE,
       FALSE, FALSE, FALSE,
	 TRUE, TRUE, TRUE,
	 FALSE, FALSE, FALSE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE,
	 FALSE, FALSE, FALSE,
	 FALSE, FALSE, FALSE },

     { "Hel",
       FALSE,	TRUE,	TRUE,
	 TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE,
	 TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE,
	 FALSE, FALSE, FALSE,
	 TRUE, TRUE, TRUE
     },

     { "Loki",
        FALSE,	TRUE,	TRUE,
	FALSE, FALSE, TRUE, TRUE, FALSE, TRUE, FALSE,
	FALSE, FALSE, TRUE, TRUE, FALSE, TRUE, FALSE,
	FALSE, FALSE, FALSE,
	FALSE, FALSE, FALSE,
	TRUE, TRUE, TRUE,
	TRUE, TRUE, TRUE,
	FALSE, FALSE, FALSE,
	TRUE, TRUE, TRUE,
	FALSE, FALSE, FALSE  },

     { "Thor",
       TRUE,	TRUE,	FALSE,
	 FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, FALSE,
	 FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, FALSE,
	 FALSE, FALSE, FALSE,
	 TRUE, TRUE, TRUE,
	 FALSE, FALSE, FALSE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE,
	 FALSE, FALSE, FALSE,
	 FALSE, FALSE, FALSE
     },

     { "Forseti",
       TRUE,	TRUE,		FALSE,
       TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE
     },

     { "Frey",
       TRUE,	TRUE,		TRUE,
	 TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE,
	 TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE,
	 FALSE, FALSE, FALSE,
	 FALSE, FALSE, FALSE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, FALSE
     },

     { "Freya",
       TRUE,      FALSE,	FALSE,
       FALSE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE,
       FALSE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE,
       FALSE, FALSE, FALSE,
	 TRUE, TRUE, TRUE,
	 FALSE, FALSE, FALSE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE },

     { "Frigg",
       TRUE,	TRUE,		TRUE,
       TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, FALSE,
       TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, FALSE,
       TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE,
	 FALSE, FALSE, FALSE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE,
	 FALSE, FALSE, FALSE },

     { "Jord",
       TRUE,	TRUE,		TRUE,
	 TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE
     },

     { "Njord",
       TRUE,      TRUE,       TRUE,
       TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE
     },

     { "Sif",
       TRUE,      TRUE, FALSE,
       TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, FALSE,
       TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, FALSE,
       TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE,
       FALSE, FALSE, FALSE,
       TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE,
       FALSE, FALSE, FALSE
     },

     { "Snotra",
       TRUE,      TRUE, TRUE,
       TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE,
       TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE,
       TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE,
       FALSE, FALSE, FALSE,
       FALSE, FALSE, FALSE,
       TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE,
       FALSE, FALSE, FALSE
     },

     { "Tyr",
       TRUE,      TRUE, FALSE,
       FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, FALSE,
       FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, FALSE,
       FALSE, FALSE, FALSE,
	 TRUE, TRUE, TRUE,
	 FALSE, FALSE, FALSE,
	 TRUE, TRUE, TRUE,
	 TRUE, TRUE, TRUE,
	 FALSE, FALSE, FALSE,
	 FALSE, FALSE, FALSE
     },

     { "Ull",
       TRUE, TRUE, TRUE,
       FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE,
       FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE,
       FALSE, FALSE, FALSE,
       FALSE, FALSE, FALSE,
       TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE,
       TRUE, TRUE, TRUE,
       FALSE, FALSE, FALSE,
       FALSE, FALSE, FALSE
     },
};

