/******************************************************************************
 * Projekt - Zaklady pocitacove grafiky - IZG
 * spanel@fit.vutbr.cz
 *
 * $Id:xormos00
 */

#include "student.h"
#include "transform.h"
#include "fragment.h"

#include <memory.h>
#include <math.h>


/*****************************************************************************
 * Globalni promenne a konstanty
 */

/* Typ/ID rendereru (nemenit) */
const int           STUDENT_RENDERER = 1;

float               n = 1.0;
const S_Material    MAT_WHITE_AMBIENT = {1.0, 1.0, 1.0, 1.0};
const S_Material    MAT_WHITE_DIFFUSE = {1.0, 1.0, 1.0, 1.0};
const S_Material    MAT_WHITE_SPECULAR = {1.0, 1.0, 1.0, 1.0};

/*****************************************************************************
 * Funkce vytvori vas renderer a nainicializuje jej
 */

S_Renderer * studrenCreate()
{
    S_StudentRenderer * renderer = (S_StudentRenderer *)malloc(sizeof(S_StudentRenderer));
    IZG_CHECK(renderer, "Cannot allocate enough memory");

    /* inicializace default rendereru */
    renderer->base.type = STUDENT_RENDERER;
    renInit(&renderer->base);

    /* nastaveni ukazatelu na upravene funkce */
    //renderer->base.type = 1;
    renderer->base.releaseFunc = studrenRelease;
    renderer->base.projectTriangleFunc = studrenProjectTriangle;

    /* inicializace nove pridanych casti */
    renderer->texture = loadBitmap(TEXTURE_FILENAME, &renderer->texture_h, &renderer->texture_w);

    return (S_Renderer *)renderer;
}

/*****************************************************************************
 * Funkce korektne zrusi renderer a uvolni pamet
 */

void studrenRelease(S_Renderer **ppRenderer)
{
    S_StudentRenderer * renderer;

    if( ppRenderer && *ppRenderer )
    {
        /* ukazatel na studentsky renderer */
        renderer = (S_StudentRenderer *)(*ppRenderer);

        /* pripadne uvolneni pameti */
        free(renderer->texture);

        /* fce default rendereru */
        renRelease(ppRenderer);
    }
}

/******************************************************************************
 * Nova fce pro rasterizaci trojuhelniku s podporou texturovani
 * Upravte tak, aby se trojuhelnik kreslil s texturami
 * (doplnte i potrebne parametry funkce - texturovaci souradnice, ...)
 * v1, v2, v3 - ukazatele na vrcholy trojuhelniku ve 3D pred projekci
 * n1, n2, n3 - ukazatele na normaly ve vrcholech ve 3D pred projekci
 * x1, y1, ... - vrcholy trojuhelniku po projekci do roviny obrazovky
 */

void studrenDrawTriangle(S_Renderer *pRenderer,
                         S_Coords *v1, S_Coords *v2, S_Coords *v3,
                         S_Coords *n1, S_Coords *n2, S_Coords *n3,
                         S_Coords *t1, S_Coords *t2, S_Coords *t3,
                         int x1, int y1,
                         int x2, int y2,
                         int x3, int y3,
                         float sh_1, float sh_2, float sh_3
                         )
{
  int         minx, miny, maxx, maxy;
  int         a1, a2, a3, b1, b2, b3, c1, c2, c3;
  int         s1, s2, s3;
  int         x, y, e1, e2, e3;
  double      alpha, beta, gamma, w1, w2, w3, z;
  double      u, v;
  S_RGBA      col1, col2, col3;
  S_RGBA      tmpColor, color;

  IZG_ASSERT(pRenderer && v1 && v2 && v3 && n1 && n2 && n3);

  /* vypocet barev ve vrcholech */
  col1 = pRenderer->calcReflectanceFunc(pRenderer, v1, n1);
  col2 = pRenderer->calcReflectanceFunc(pRenderer, v2, n2);
  col3 = pRenderer->calcReflectanceFunc(pRenderer, v3, n3);

  /* obalka trojuhleniku */
  minx = MIN(x1, MIN(x2, x3));
  maxx = MAX(x1, MAX(x2, x3));
  miny = MIN(y1, MIN(y2, y3));
  maxy = MAX(y1, MAX(y2, y3));

  /* oriznuti podle rozmeru okna */
  miny = MAX(miny, 0);
  maxy = MIN(maxy, pRenderer->frame_h - 1);
  minx = MAX(minx, 0);
  maxx = MIN(maxx, pRenderer->frame_w - 1);

  /* Pineduv alg. rasterizace troj.
     hranova fce je obecna rovnice primky Ax + By + C = 0
     primku prochazejici body (x1, y1) a (x2, y2) urcime jako
     (y1 - y2)x + (x2 - x1)y + x1y2 - x2y1 = 0 */

  /* normala primek - vektor kolmy k vektoru mezi dvema vrcholy, tedy (-dy, dx) */
  a1 = y1 - y2;
  a2 = y2 - y3;
  a3 = y3 - y1;
  b1 = x2 - x1;
  b2 = x3 - x2;
  b3 = x1 - x3;

  /* koeficient C */
  c1 = x1 * y2 - x2 * y1;
  c2 = x2 * y3 - x3 * y2;
  c3 = x3 * y1 - x1 * y3;

  /* vypocet hranove fce (vzdalenost od primky) pro protejsi body */
  s1 = a1 * x3 + b1 * y3 + c1;
  s2 = a2 * x1 + b2 * y1 + c2;
  s3 = a3 * x2 + b3 * y2 + c3;

  if ( !s1 || !s2 || !s3 )
  {
      return;
  }

  /* normalizace, aby vzdalenost od primky byla kladna uvnitr trojuhelniku */
  if( s1 < 0 )
  {
      a1 *= -1;
      b1 *= -1;
      c1 *= -1;
  }
  if( s2 < 0 )
  {
      a2 *= -1;
      b2 *= -1;
      c2 *= -1;
  }
  if( s3 < 0 )
  {
      a3 *= -1;
      b3 *= -1;
      c3 *= -1;
  }

  /* koeficienty pro barycentricke souradnice */
  alpha = 1.0 / ABS(s2);
  beta = 1.0 / ABS(s3);
  gamma = 1.0 / ABS(s1);

  /* vyplnovani... */
  for( y = miny; y <= maxy; ++y )
  {
      /* inicilizace hranove fce v bode (minx, y) */
      e1 = a1 * minx + b1 * y + c1;
      e2 = a2 * minx + b2 * y + c2;
      e3 = a3 * minx + b3 * y + c3;

      for( x = minx; x <= maxx; ++x )
      {
          if( e1 >= 0 && e2 >= 0 && e3 >= 0 )
          {
              /* interpolace pomoci barycentrickych souradnic
                 e1, e2, e3 je aktualni vzdalenost bodu (x, y) od primek */
              w1 = alpha * e2;
              w2 = beta * e3;
              w3 = gamma * e1;

              /* Interpolacia u,v suradnice KROK C.4 A KROK C.6 */
              u = (w1 * t1->x/sh_1 + w2 * t2->x/sh_2 + w3 * t3->x/sh_3) /
                  (w1 * 1/sh_1 + w2 * 1/sh_2 + w3 * 1/sh_3);

              v = (w1 * t1->y/sh_1 + w2 * t2->y/sh_2 + w3 * t3->y/sh_3) /
                  (w1 * 1/sh_1 + w2 * 1/sh_2 + w3 * 1/sh_3);
              // u =  w1 * t1->x + w2 * t2->x + w3 * t3->x;
              // v =  w1 * t1->y + w2 * t2->y + w3 * t3->y;

              /* interpolace z-souradnice */
              z = w1 * v1->z + w2 * v2->z + w3 * v3->z;

              /* interpolacia farby */
              color.red = ROUND2BYTE(w1 * col1.red + w2 * col2.red + w3 * col3.red);
              color.green = ROUND2BYTE(w1 * col1.green + w2 * col2.green + w3 * col3.green);
              color.blue = ROUND2BYTE(w1 * col1.blue + w2 * col2.blue + w3 * col3.blue);
              color.alpha = 255;

              /* Priprava pre textovacie suradnice KROK C. 4 */
              tmpColor = studrenTextureValue((S_StudentRenderer *) pRenderer, u, v);

              color.red = (color.red * tmpColor.red) / color.alpha;
              color.green = (color.green * tmpColor.green) / color.alpha;
              color.blue = (color.blue * tmpColor.blue) / color.alpha;

              /* vykresleni bodu */
              if( z < DEPTH(pRenderer, x, y) )
              {
                  PIXEL(pRenderer, x, y) = color;
                  DEPTH(pRenderer, x, y) = z;
              }
              //studrenTextureValue((S_StudentRenderer *)pRenderer, u, v);
          }

          /* hranova fce o pixel vedle */
          e1 += a1;
          e2 += a2;
          e3 += a3;
      }
  }
}

/******************************************************************************
 * Vykresli i-ty trojuhelnik n-teho klicoveho snimku modelu
 * pomoci nove fce studrenDrawTriangle()
 * Pred vykreslenim aplikuje na vrcholy a normaly trojuhelniku
 * aktualne nastavene transformacni matice!
 * Upravte tak, aby se model vykreslil interpolovane dle parametru n
 * (cela cast n udava klicovy snimek, desetinna cast n parametr interpolace
 * mezi snimkem n a n + 1)
 * i - index trojuhelniku
 * n - index klicoveho snimku (float pro pozdejsi interpolaci mezi snimky)
 */

void studrenProjectTriangle(S_Renderer *pRenderer, S_Model *pModel, int i, float n)
{
  S_Coords    aa, bb, cc;             /* souradnice vrcholu po transformaci */
  S_Coords    naa, nbb, ncc;          /* normaly ve vrcholech po transformaci */
  S_Coords    nn;                     /* normala trojuhelniku po transformaci */
  int         u1, v1, u2, v2, u3, v3; /* souradnice vrcholu po projekci do roviny obrazovky */
  S_Triangle  * triangle;
  S_Triangle  * triangleNext;
  int         vertexOffset, normalOffset; /* offset pro vrcholy a normalove vektory trojuhelniku */
  int         i0, i1, i2, in;             /* indexy vrcholu a normaly pro i-ty trojuhelnik n-teho snimku */

  int         i0Next, i1Next, i2Next, inNext;
  int         vertexOffsetNext, normalOffsetNext;

  float       sh_1, sh_2, sh_3;

  float n0, n1, n2, n_n;
  float mod_n;

  IZG_ASSERT(pRenderer && pModel && i >= 0 && i < trivecSize(pModel->triangles) && n >= 0 );

  /* z modelu si vytahneme i-ty trojuhelnik */
  triangle = trivecGetPtr(pModel->triangles, i);

  if (i == (trivecSize(pModel->triangles) - 1)) {
    i = 0;
  }
    triangleNext = trivecGetPtr(pModel->triangles, i+1);


  /* ziskame offset pro vrcholy n-teho snimku */
  vertexOffset = (((int) n) % pModel->frames) * pModel->verticesPerFrame;

  /* ziskame offset pro normaly trojuhelniku n-teho snimku */
  normalOffset = (((int) n) % pModel->frames) * pModel->triangles->size;

  vertexOffsetNext = (((int) n+1) % pModel->frames) * pModel->verticesPerFrame;
  normalOffsetNext = (((int) n+1) % pModel->frames) * pModel->triangles->size;

  /* indexy vrcholu pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
  i0 = triangle->v[ 0 ] + vertexOffset;
  i1 = triangle->v[ 1 ] + vertexOffset;
  i2 = triangle->v[ 2 ] + vertexOffset;

  /* index normaloveho vektoru pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
  in = triangle->n + normalOffset;

  i0Next = triangleNext->v[ 0 ] + vertexOffsetNext;
  i1Next = triangleNext->v[ 1 ] + vertexOffsetNext;
  i2Next = triangleNext->v[ 2 ] + vertexOffsetNext;

  inNext = triangleNext->n + normalOffsetNext;

  mod_n = n - (int)n;

  /* Linearna interpolacia medzi snimkami KROK C.3 */
  n0 = i0 + mod_n*(i0Next - i0);
  n1 = i1 + mod_n*(i1Next - i1);
  n2 = i2 + mod_n*(i2Next - i2);
  n_n = in + mod_n*(inNext - in);

  /* transformace vrcholu matici model */
  trTransformVertex(&aa, cvecGetPtr(pModel->vertices, n0));
  trTransformVertex(&bb, cvecGetPtr(pModel->vertices, n1));
  trTransformVertex(&cc, cvecGetPtr(pModel->vertices, n2));

  /* promitneme vrcholy trojuhelniku na obrazovku */
  sh_1 = trProjectVertex(&u1, &v1, &aa);
  sh_2 = trProjectVertex(&u2, &v2, &bb);
  sh_3 = trProjectVertex(&u3, &v3, &cc);

  /* pro osvetlovaci model transformujeme take normaly ve vrcholech */
  trTransformVector(&naa, cvecGetPtr(pModel->normals, n0));
  trTransformVector(&nbb, cvecGetPtr(pModel->normals, n1));
  trTransformVector(&ncc, cvecGetPtr(pModel->normals, n2));

  /* normalizace normal */
  coordsNormalize(&naa);
  coordsNormalize(&nbb);
  coordsNormalize(&ncc);

  /* transformace normaly trojuhelniku matici model */
  trTransformVector(&nn, cvecGetPtr(pModel->trinormals, n_n));

  /* normalizace normaly */
  coordsNormalize(&nn);

  /* je troj. privraceny ke kamere, tudiz viditelny? */
  if( !renCalcVisibility(pRenderer, &aa, &nn) )
  {
      /* odvracene troj. vubec nekreslime */
      return;
  }

  /* rasterizace trojuhelniku */
  studrenDrawTriangle(pRenderer,
                  &aa, &bb, &cc,
                  &naa, &nbb, &ncc,
                  &triangle->t[0],
                  &triangle->t[1],
                  &triangle->t[2],
                  u1, v1, u2, v2, u3, v3,
                  sh_1, sh_2, sh_3
                  );
}

/******************************************************************************
* Vraci hodnotu v aktualne nastavene texture na zadanych
* texturovacich souradnicich u, v
* Pro urceni hodnoty pouziva bilinearni interpolaci
* Pro otestovani vraci ve vychozim stavu barevnou sachovnici dle uv souradnic
* u, v - texturovaci souradnice v intervalu 0..1, ktery odpovida sirce/vysce textury
*/

S_RGBA studrenTextureValue( S_StudentRenderer * pRenderer, double u, double v )
{
  // http://cs.wikipedia.org/wiki/Biline%C3%A1rn%C3%AD_interpolace
  // BILINEARNA INTERPOLACIA KROK C.5
  S_RGBA      color, z1, z2, z3 ,z4;
  double      j, i;
  int         x1, x2, y1, y2;

  i = (u * pRenderer->texture_h);
  j = (v * pRenderer->texture_w);

  x1 = i;
  x2 = i+1;
  y1 = j;
  y2 = j+1;

  z1 = pRenderer->texture[pRenderer->texture_w * x1 + y1];
  z2 = pRenderer->texture[pRenderer->texture_w * x1 + y2];
  z3 = pRenderer->texture[pRenderer->texture_w * x2 + y1];
  z4 = pRenderer->texture[pRenderer->texture_w * x2 + y2];

  color.red = z1.red * (1 - u) * (1 - v)
            + z2.red * u * (1-v)
            + z3.red * (1 - u) * v
            + z4.red * (u * v);

  color.green = z1.green * (1 - u) * (1 - v)
              + z2.green * u * (1 - v)
              + z3.green * (1 - u) * v
              + z4.green * (u * v);

  color.blue = z1.blue * (1 - u) * (1 - v)
             + z2.blue * u * (1 - v)
             + z3.blue * (1 - u) * v
             + z4.blue * (u * v);

  color.alpha = z1.alpha * (1 - u) * (1 - v)
              + z2.alpha * u * (1 - v)
              + z3.alpha * (1 - u) * v
              + z4.alpha * (u * v);

  return color;
}

/******************************************************************************
 ******************************************************************************
 * Funkce pro vyrenderovani sceny, tj. vykresleni modelu
 * Upravte tak, aby se model vykreslil animovane
 * (volani renderModel s aktualizovanym parametrem n)
 */

void renderStudentScene(S_Renderer *pRenderer, S_Model *pModel)
{
  /* test existence frame bufferu a modelu */
  IZG_ASSERT(pModel && pRenderer);

  /* nastavit projekcni matici */
  trProjectionPerspective(pRenderer->camera_dist, pRenderer->frame_w, pRenderer->frame_h);

  /* vycistit model matici */
  trLoadIdentity();

  /* nejprve nastavime posuv cele sceny od/ke kamere */
  trTranslate(0.0, 0.0, pRenderer->scene_move_z);

  /* nejprve nastavime posuv cele sceny v rovine XY */
  trTranslate(pRenderer->scene_move_x, pRenderer->scene_move_y, 0.0);

  /* natoceni cele sceny - jen ve dvou smerech - mys je jen 2D... :( */
  trRotateX(pRenderer->scene_rot_x);
  trRotateY(pRenderer->scene_rot_y);

  /* nastavime material */
  renMatAmbient(pRenderer, &MAT_WHITE_AMBIENT);
  renMatDiffuse(pRenderer, &MAT_WHITE_DIFFUSE);
  renMatSpecular(pRenderer, &MAT_WHITE_SPECULAR);

  /* a vykreslime nas model (ve vychozim stavu kreslime pouze snimek 0) */
  renderModel(pRenderer, pModel, n);
}

/* Callback funkce volana pri tiknuti casovace
 * ticks - pocet milisekund od inicializace */
void onTimer( int ticks )
{
    /* KROK C.2*/
    /* Zobrazenie snimkov */
    n = (int)ticks / 90;
}

/*****************************************************************************
 *****************************************************************************/
