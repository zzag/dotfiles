using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace astarconsole
{
    class Program
    {
        static void Main(string[] args)
        {
            AStar.Field field = new AStar.Field(new int[,] {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
            });

            AStar pathfinder = new AStar(field);
            AStar.Point start = new AStar.Point(0, 0);
            AStar.Point end = new AStar.Point(11, 5);
            List<AStar.Point> path = pathfinder.getPath(start, end);

            for (int i = 0; i < field.height(); i++)
            {
                for(int j = 0 ; j < field.width(); j++)
                {
                    bool drawDot = true;

                    foreach(AStar.Point t in path)
                    {
                        if (t.m_x == i && t.m_y == j)
                            drawDot = false;
                    }


                    if (i == start.m_x && j == start.m_y ||
                        i == end.m_x && end.m_y == j)
                    {
                        Console.Write("O ");
                    }
                    else if (field[i, j] == 1)
                        Console.Write("# ");
                    else if (drawDot == true)
                        Console.Write(". ");
                    else
                        Console.Write("* ");
                }

                Console.Write("\n");
            }

            Console.Read();
        }
    }
}
