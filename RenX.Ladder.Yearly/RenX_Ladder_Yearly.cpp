/**
 * Copyright (C) 2016-2017 Jessica James.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Written by Jessica James <jessica.aj@outlook.com>
 */

#include <ctime>
#include "Jupiter/IRC_Client.h"
#include "RenX_Ladder_Yearly.h"

using namespace Jupiter::literals;

bool RenX_Ladder_Yearly_TimePlugin::initialize()
{
	// Load database
	this->database.process_file(this->config.get("LadderDatabase"_jrs, "Ladder.Yearly.db"_jrs));
	this->database.setName(this->config.get("DatabaseName"_jrs, "Yearly"_jrs));
	this->database.setOutputTimes(this->config.get<bool>("OutputTimes"_jrs, false));

	this->last_sorted_year = gmtime(std::addressof<const time_t>(time(0)))->tm_year;
	this->database.OnPreUpdateLadder = OnPreUpdateLadder;

	// Force database to default, if desired
	if (this->config.get<bool>("ForceDefault"_jrs, false))
		RenX::default_ladder_database = &this->database;

	return true;
}

// Plugin instantiation and entry point.
RenX_Ladder_Yearly_TimePlugin pluginInstance;

void OnPreUpdateLadder(RenX::LadderDatabase &database, RenX::Server &, const RenX::TeamType &)
{
	tm *tm_ptr = gmtime(std::addressof<const time_t>(time(0)));
	if (pluginInstance.last_sorted_year != tm_ptr->tm_year)
		database.erase();
	pluginInstance.last_sorted_year = tm_ptr->tm_year;
}

extern "C" __declspec(dllexport) Jupiter::Plugin *getPlugin()
{
	return &pluginInstance;
}
