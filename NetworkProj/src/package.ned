package mesh;

@license(LGPL);

import ned.IdealChannel;

network Mesh
{
    parameters:
        int n = default(5);
    submodules:
        nodes[n]: Node;
        hubs: Hub;
    connections: 
       for i=0..sizeof(nodes)-1
       {
           hubs.outs++ --> IdealChannel --> nodes[i].ins;
           hubs.ins++ <-- IdealChannel <-- nodes[i].outs;
       }  
}
