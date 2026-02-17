/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: hash.cc,v 1.1.1.1 1996/04/16 18:51:57 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#include <ifound/osdep.h>

#ifdef __GNUC__
#pragma implementation "hashtab.h"
#pragma implementation "bhashtab.h"
#endif

#include <ifound/hashtab.h>
#include <ifound/itypes.h>
#include <ifound/stdc.h>

HashNode *BaseHashTab::addNode(HashNode *node)
{
	Int hash_value = f_hash(node + 1, modulo);
	assert(hash_value >= 0 && hash_value < modulo);
	HashNode **pcurr = &tab[hash_value];
	Int comp = 1;

#ifdef DEBUG
	++nAdds;
	while (*pcurr && (++nCmps, (comp = f_cmp(node + 1, (*pcurr) + 1))) > 0)
		pcurr = &(*pcurr)->next;
#else
	while (*pcurr && (comp = f_cmp(node + 1, (*pcurr) + 1)) > 0)
		pcurr = &(*pcurr)->next;
#endif

	if (comp)
		node->next = *pcurr;
	else {
		node->next = (*pcurr)->next;
		f_del(*pcurr);
	}
	return *pcurr = node;
}

void BaseHashTab::delNode(const HashNode &node)
{
	Int hash_value = f_hash(&node + 1, modulo);
	assert(hash_value >= 0 && hash_value < modulo);
	HashNode **pcurr = &tab[hash_value];
	Int comp = 1;

#ifdef DEBUG
	++nDels;
	while (*pcurr && (++nCmps, (comp = f_cmp(&node + 1, (*pcurr) + 1))) > 0)
		pcurr = &(*pcurr)->next;
#else
	while (*pcurr && (comp = f_cmp(&node + 1, (*pcurr) + 1)) > 0)
		pcurr = &(*pcurr)->next;
#endif

	if (!comp) {
		HashNode *del_n = *pcurr;
		*pcurr = (*pcurr)->next;
		f_del(del_n);
	}
}

HashNode *BaseHashTab::findNode(void *node) const
{
	Int hash_value = f_hash(node, modulo);
	assert(hash_value >= 0 && hash_value < modulo);
	register HashNode *pcurr = tab[hash_value];
	register Int comp = 1;

#ifdef DEBUG
	++nFinds;
	while (pcurr && (++nCmps, (comp = f_cmp(node, pcurr + 1))) > 0)
		pcurr = pcurr->next;
#else
	while (pcurr && (comp = f_cmp(node, pcurr + 1)) > 0)
		pcurr = pcurr->next;
#endif

	return !comp ? pcurr : NULL;
}

BaseHashTab::BaseHashTab(Int m)
{
	m += m/2;

	if (m%2 == 0)
		m++;

	while (m%3 == 0 || m%5 == 0 || m%7 == 0)
		m += 2;

	tab = tr_new HashNode*[modulo = m];
	for (HashNode **n = tab; n < &tab[modulo]; n++)
		*n = NULL;
}

BaseHashTab::~BaseHashTab()
{
	if (tab != NULL) {
		for (HashNode **np = tab; np < &tab[modulo]; np++) {
			if (*np) {
				HashNode *currNode, *nextNode;
				for (currNode = *np; currNode; currNode = nextNode) {
					nextNode = currNode->next;
					f_del(currNode);
				}
			}
		}
		tr_delete_vec(tab);
	}
}

#ifdef DEBUG
#include <stdio.h>

Int BaseHashTab::min = INT_MAX;
Int BaseHashTab::max = INT_MIN;
Int BaseHashTab::nFinds = 0;
Int BaseHashTab::nDels = 0;
Int BaseHashTab::nAdds = 0;
Int BaseHashTab::nCmps = 0;
Int BaseHashTab::nElems = 0;
Int BaseHashTab::nBuckets = 0;

void BaseHashTab::resetStats()
{
	min = INT_MAX;
	max = INT_MIN;
	nElems = nBuckets = nFinds = nDels = nAdds = nCmps = 0;
}

void BaseHashTab::computeStats(BaseHashTab *ht)
{
	int	i, n;

	HashNode *pcurr;

	for (i = 0; i < ht->modulo; i++) {
		n = 0;
		pcurr = ht->buckets()[i];
		while (pcurr) {
			n++;
			pcurr = pcurr->next;
		}
		if (n < min)
			min = n;
		if (n > max)
			max = n;
		nBuckets++;
		nElems += n;
	}
}

void BaseHashTab::printStats()
{
	fprintf(stderr, "BaseHashTab Statistics.\n");
	fprintf(stderr, "Min. elems/bucket: %8ld\n", min);
	fprintf(stderr, "Max. elems/bucket: %8ld\n", max);
	fprintf(stderr, "Avg. elems/bucket: %11.2f\n",
			nElems ? ((double) nElems)/((double) nBuckets) : 0.0);
	fprintf(stderr, "# Adds           : %8ld\n", nAdds);
	fprintf(stderr, "# Deletes        : %8ld\n", nDels);
	fprintf(stderr, "# Finds          : %8ld\n", nFinds);
	fprintf(stderr, "# Compares       : %8ld\n", nCmps);
	fprintf(stderr, "Compares/op.     : %11.2f\n",
			nCmps ? ((double) (nAdds + nDels + nFinds))/((double) nCmps) :
			0.0);
}
#endif

// end of hash
