
#include <cstdlib>

static int	ft_o(char c1, char c2)
{
	if (c1 == c2)
		return (1);
	else
		return (0);
}

static int	ft_count_wrd(char const *s, char c)
{
	int	wrdcount;
	int	i;

	i = 1;
	wrdcount = 0;
	if (!s[0])
		return (wrdcount);
	while (1)
	{
		if ((ft_o(s[i], c) && !ft_o(s[i - 1], c))
			|| (!s[i] && !ft_o(s[i - 1], c)))
			wrdcount++;
		if (!s[i])
			break ;
		i++;
	}
	return (wrdcount);
}

static char	*ft_get_wrd(char const *s, int e)
{
	char	*str;
	int		i;

	i = -1;
	str = reinterpret_cast<char *>(malloc(sizeof(char) * (e + 1)));
	if (!str)
		return (NULL);
	while (++i < e)
		str[i] = s[i];
	str[i] = 0;
	return (str);
}

static char	*ft_find_wrd(char const *s, char set, int wordneeded)
{
	int	i;
	int	e;

	i = 0;
	e = 0;
	while (i <= wordneeded)
	{
		while (ft_o(*s, set))
			s++;
		while (!ft_o(s[e], set) && s[e])
			e++;
		if (i == wordneeded)
			return (ft_get_wrd(s, e));
		s += e;
		e = 0;
		i++;
	}
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	int		nbwrd;
	int		i;

	nbwrd = ft_count_wrd(s, c);
	strs = reinterpret_cast<char **>(malloc(sizeof(char *) * (nbwrd + 1)));
	if (!strs)
		return (NULL);
	i = -1;
	while (++i < nbwrd)
		*(strs + i) = ft_find_wrd(s, c, i);
	*(strs + i) = NULL;
	return (strs);
}
