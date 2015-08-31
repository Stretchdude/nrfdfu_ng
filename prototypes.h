/* nrfdfu.c */
extern int main(int argc, char *argv[]);
/* util.c */
extern void *xmalloc(size_t s);
extern void *xrealloc(void *p, size_t s);
/* zip.c */
extern void fatal_zip(struct zip *zip);
extern struct zip *open_zip(const char *fn);
extern size_t read_file_from_zip(struct zip *zip, const char *fn, void *_buf);
/* ble.c */
extern int bt_thing(const char *bdaddr);
/* manifest.c */
extern json_object *_json_object_object_get(json_object *obj, const char *name);
extern struct manifest *parse_manifest(const char *str);
/* dfu.c */
extern void dfu(const char *bdaddr, const char *type, const char *version, uint8_t *dat, size_t dat_sz, uint8_t *bin, size_t bin_sz);
