#include "project.h"


json_object *
_json_object_object_get (json_object * obj, const char *name)
{
  json_object *sub;
  return json_object_object_get_ex (obj, name, &sub) ? sub : NULL;
}


static void
dump_manifest (struct manifest *m)
{
  printf ("Manifest:\n");
  printf ("   type       : %s\n", m->type);
  printf ("   dfu_version: %s\n", m->dfu_version);
  printf ("   dat_file   : %s\n", m->dat_file);
  printf ("   bin_file   : %s\n", m->bin_file);
}



struct manifest *
parse_manifest (const char *str)
{
  json_object *json, *manifest;
  enum json_tokener_error jerr = json_tokener_success;
  struct manifest *m;


  m = xmalloc (sizeof (*m));
  memset (m, 0, sizeof (*m));

  json = json_tokener_parse_verbose (str, &jerr);

  manifest = _json_object_object_get (json, "manifest");

  m->dfu_version =
    json_object_get_string (_json_object_object_get
                            (manifest, "dfu_version"));

  json_object_object_foreach (manifest, key, val)
  {
    if (!strcmp (key, "dfu_version"))
      continue;

    m->type = key;

    m->bin_file =
      json_object_get_string (_json_object_object_get (val, "bin_file"));
    m->dat_file =
      json_object_get_string (_json_object_object_get (val, "dat_file"));
    break;
  }


  dump_manifest (m);


  if (!m->dfu_version || !m->type || !m->bin_file || !m->dat_file)
    {
      fprintf (stderr, "Failed to process manifest\n");
      exit (EXIT_FAILURE);
    }





  return m;
}
