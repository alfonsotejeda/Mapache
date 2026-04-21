#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void tree(const char *path, int depth)
{
    DIR *dir;
    struct dirent *entry;
    struct stat sb;
    char fullpath[4096];
    int i;

    dir = opendir(path);
    if (dir == NULL) {
        printf("tree: cannot open %s\n", path);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {

        /* ignorar . y .. */
        if (strcmp(entry->d_name, ".") == 0) continue;
        if (strcmp(entry->d_name, "..") == 0) continue;

        /* construir ruta completa */
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        /* obtener info del archivo sin seguir symlinks */
        if (lstat(fullpath, &sb) != 0) continue;

        /* imprimir indentacion */
        for (i = 0; i < depth; i++)
            printf("---");

        /* imprimir nombre */
        if (S_ISLNK(sb.st_mode)) {
            printf("-> %s\n", entry->d_name);
        } else if (S_ISDIR(sb.st_mode)) {
            printf("[%s]\n", entry->d_name);
            tree(fullpath, depth + 1);
        } else {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{
    const char *path;

    if (argc < 2)
        path = ".";
    else
        path = argv[1];

    printf("%s\n", path);
    tree(path, 0);

    return 0;
}
