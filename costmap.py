import re
import numpy

class CostMap():

    def __init__(self,filename):
        self.LETHAL_OBSTACLE=255
        self.INSCRIBED_INFLATED_OBSTACLE=254
        self.occmap_arr=read_pgm(filename)
        self.costmap_arr=self.compute_costmap(self.occmap_arr)

    def compute_cost(self,distance):
        """
        """
        resolution_=0.050000 # map resolution from the yaml
        LETHAL_OBSTACLE=255 #
        INSCRIBED_INFLATED_OBSTACLE=254 #
        inscribed_radius_=0.25 #radius of the robot
        weight_=5

        cost = 0;
        if (distance == 0):
            cost = LETHAL_OBSTACLE
        elif (distance * resolution_ <= inscribed_radius_):
            cost = INSCRIBED_INFLATED_OBSTACLE
        else:
            #make sure cost falls off by Euclidean distance
            euclidean_distance = distance * resolution_;
            factor = numpy.exp(-1.0 * weight_ * (euclidean_distance - inscribed_radius_));
            cost = ((INSCRIBED_INFLATED_OBSTACLE - 1) * factor);

        return cost

    def compute_costmap(self,occ_map):
        """
        """
        costmap = numpy.ones_like(occ_map)*255
        free_thresh = 0.196
        for x_idx in range(occ_map.shape[0]):
            for y_idx in range(occ_map.shape[1]):
                if occ_map[x_idx,y_idx]<free_thresh:
                    costmap[x_idx,y_idx]=self.compute_cost(distance((x_idx,y_idx), occ_map))

        return costmap

def read_pgm(filename, byteorder='>'):
    """
    Return image data from a raw PGM file as numpy array.
    Format specification: http://netpbm.sourceforge.net/doc/pgm.html
    """
    with open(filename, 'rb') as f:
        buffer = f.read()
    try:
        header, width, height, maxval = re.search(
            b"(^P5\s(?:\s*#.*[\r\n])*"
            b"(\d+)\s(?:\s*#.*[\r\n])*"
            b"(\d+)\s(?:\s*#.*[\r\n])*"
            b"(\d+)\s(?:\s*#.*[\r\n]\s)*)", buffer).groups()
    except AttributeError:
        raise ValueError("Not a raw PGM file: '%s'" % filename)
    return (255-numpy.frombuffer(buffer,
                            dtype='u1' if int(maxval) < 256 else byteorder+'u2',
                            count=int(width)*int(height),
                            offset=len(header)
                            ).reshape((int(height), int(width))))/255.0

def circle(radius,center=(0,0)):
    '''
    Bresenham circle algorithm
    '''
    # init vars
    switch = 3 - (2 * radius)
    points = set() #set instead of list to avoid duplicates
    x = 0
    y = radius
    # first quarter/octant starts clockwise at 12 o'clock
    while x <= y:
        # first quarter first octant
        points.add((center[0]+x, center[1]-y))
        # first quarter 2nd octant
        points.add((center[0]+y, center[1]-x))
        # second quarter 3rd octant
        points.add((center[0]+y, center[1]+x))
        # second quarter 4.octant
        points.add((center[0]+x, center[1]+y))
        # third quarter 5.octant
        points.add((center[0]-x, center[1]+y))
        # third quarter 6.octant
        points.add((center[0]-y, center[1]+x))
        # fourth quarter 7.octant
        points.add((center[0]-y, center[1]-x))
        # fourth quarter 8.octant
        points.add((center[0]-x, center[1]-y))
        if switch < 0:
            switch = switch + (4 * x) + 6
        else:
            switch = switch + (4 * (x - y)) + 10
            y = y - 1
        x = x + 1
    return points

def distance(current_cell,npmap):
    """
    """
    dist=0
    cells=circle(dist,current_cell)
    cell_vals=take_np(npmap,list(cells))
    occ_tresh=0.9

    while all(val < occ_tresh for val in cell_vals):
        dist=dist+1
        cells=circle(dist,current_cell)
        cell_vals=take_np(npmap,list(cells))

    return dist

def take_np(arr,idx):
    """
    """
    out=[]
    for i in idx:
        out.append(arr[i[0],i[1]])
    return out
