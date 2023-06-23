/*
 *  OpenSlide, a library for reading whole slide image files
 *
 *  Copyright (c) 2010-2012 Carnegie Mellon University
 *  All rights reserved.
 *
 *  OpenSlide is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 2.1.
 *
 *  OpenSlide is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with OpenSlide. If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include <glib.h>
#include "openslide.h"
#include "openslide-common.h"
#include "slidetool.h"

static bool process(const char *file) {
  g_autoptr(openslide_t) osr = openslide_open(file);
  if (common_warn_on_error(osr, "%s", file)) {
    return false;
  }

  const char *hash =
    openslide_get_property_value(osr, OPENSLIDE_PROPERTY_NAME_QUICKHASH1);
  if (hash == NULL) {
    common_warn("%s: No quickhash-1 available", file);
    return false;
  }

  printf("%s  %s\n", hash, file);
  return true;
}

static int do_quickhash1sum(int narg, char **args) {
  int ret = 0;
  for (int i = 0; i < narg; i++) {
    if (!process(args[i])) {
      ret = 1;
    }
  }
  return ret;
}

const struct command quickhash1sum_cmd = {
  .parameter_string = "FILE...",
  .summary = "Print OpenSlide quickhash-1 (256-bit) checksums.",
  .options = legacy_opts,
  .min_positional = 1,
  .handler = do_quickhash1sum,
};
